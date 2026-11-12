#pragma once

// Stable-storage abstraction for the Raft simulation (raft_sim.h).
//
// Why this exists: the earlier sim's restart() "magically" kept a node's persistent state in
// the live object, so a crash never actually tested that state was durable. Real Raft assumes
// (currentTerm, votedFor, log, snapshot) survive a power loss; volatile state (role,
// commitIndex, lastApplied, the state machine) does not. This header makes that boundary
// explicit: every persistent mutation goes through a Storage, and restart() reconstructs the
// node purely from Storage::load() — so anything the code forgot to persist is genuinely lost
// and the tests catch it.
//
// Two implementations:
//   - MemStorage  : keeps the blob in memory. Survives crash()/restart() (which drop only the
//                   live Node), preserves the sim's determinism and speed. The default.
//   - FileStorage : real open/write/fsync with atomic temp+rename and a CRC32 integrity guard,
//                   so a torn or half-written file is detected and treated as "nothing durable
//                   yet" rather than silently loading garbage. Darwin has no fdatasync — we use
//                   fsync (see the F_FULLFSYNC note in leveldb_mini for the true-platter story).

#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace raft_sim {

// A replicated-log entry. Lives here (not in raft_sim.h) so Storage can serialize it without a
// circular include. (term, index) uniquely identifies an entry across the cluster.
struct LogEntry {
    int term = 0;
    std::string op;  // "put" | "delete" | "noop"
    std::string key;
    std::string value;

    bool operator==(const LogEntry& other) const {
        return term == other.term && op == other.op && key == other.key && value == other.value;
    }
};

// The complete durable image of a node. log holds only entries AFTER the snapshot boundary,
// i.e. logical indices [lastIncludedIndex+1 .. lastIncludedIndex+log.size()].
struct PersistentState {
    int currentTerm = 0;
    int votedFor = -1;
    int lastIncludedIndex = -1;  // snapshot covers [0, lastIncludedIndex]; -1 = no snapshot
    int lastIncludedTerm = 0;
    std::string snapshot;  // serialized state machine at the snapshot boundary
    std::vector<LogEntry> log;
};

class Storage {
 public:
    virtual ~Storage() = default;

    // Persist the "hard state" (term + vote). Small and frequent; separated so a real backend
    // could write it without rewriting the whole log.
    virtual void saveHardState(int currentTerm, int votedFor) = 0;
    // Persist the post-snapshot log suffix (full rewrite — truncation and append both funnel
    // here, keeping the durable log exactly equal to the in-memory suffix).
    virtual void saveLog(const std::vector<LogEntry>& log) = 0;
    // Persist a snapshot and the log suffix that remains after prefix compaction.
    virtual void saveSnapshot(int lastIncludedIndex, int lastIncludedTerm,
                              const std::string& snapshot, const std::vector<LogEntry>& log) = 0;
    // Reconstruct the durable image (default-constructed if nothing was ever stored).
    virtual PersistentState load() = 0;
};

// --- serialization (fixed little-endian, deterministic) ---------------------

namespace detail {

inline void PutU32(std::string* out, std::uint32_t v) {
    for (int i = 0; i < 4; ++i) out->push_back(static_cast<char>((v >> (8 * i)) & 0xFF));
}
inline void PutI32(std::string* out, std::int32_t v) {
    PutU32(out, static_cast<std::uint32_t>(v));
}
inline void PutStr(std::string* out, const std::string& s) {
    PutU32(out, static_cast<std::uint32_t>(s.size()));
    out->append(s);
}

inline bool GetU32(const std::string& in, std::size_t* pos, std::uint32_t* v) {
    if (*pos + 4 > in.size()) return false;
    std::uint32_t r = 0;
    for (int i = 0; i < 4; ++i)
        r |= static_cast<std::uint32_t>(static_cast<unsigned char>(in[*pos + i])) << (8 * i);
    *pos += 4;
    *v = r;
    return true;
}
inline bool GetI32(const std::string& in, std::size_t* pos, std::int32_t* v) {
    std::uint32_t u = 0;
    if (!GetU32(in, pos, &u)) return false;
    *v = static_cast<std::int32_t>(u);
    return true;
}
inline bool GetStr(const std::string& in, std::size_t* pos, std::string* s) {
    std::uint32_t len = 0;
    if (!GetU32(in, pos, &len)) return false;
    if (*pos + len > in.size()) return false;
    s->assign(in, *pos, len);
    *pos += len;
    return true;
}

// CRC32 (IEEE, reflected) — compact table-driven guard against torn/half-written files. Same
// integrity lesson as leveldb_mini's CRC32C record framing, kept self-contained here.
inline std::uint32_t Crc32(const std::string& data) {
    static const std::array<std::uint32_t, 256> table = [] {
        std::array<std::uint32_t, 256> t{};
        for (std::uint32_t i = 0; i < 256; ++i) {
            std::uint32_t c = i;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? (0xEDB88320u ^ (c >> 1)) : (c >> 1);
            t[i] = c;
        }
        return t;
    }();
    std::uint32_t crc = 0xFFFFFFFFu;
    for (unsigned char b : data) crc = table[(crc ^ b) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFu;
}

inline std::string Serialize(const PersistentState& s) {
    std::string payload;
    PutI32(&payload, s.currentTerm);
    PutI32(&payload, s.votedFor);
    PutI32(&payload, s.lastIncludedIndex);
    PutI32(&payload, s.lastIncludedTerm);
    PutStr(&payload, s.snapshot);
    PutU32(&payload, static_cast<std::uint32_t>(s.log.size()));
    for (const auto& e : s.log) {
        PutI32(&payload, e.term);
        PutStr(&payload, e.op);
        PutStr(&payload, e.key);
        PutStr(&payload, e.value);
    }
    std::string framed;
    PutU32(&framed, Crc32(payload));  // crc prefix guards the whole payload
    framed.append(payload);
    return framed;
}

inline bool Deserialize(const std::string& framed, PersistentState* out) {
    std::size_t pos = 0;
    std::uint32_t crc = 0;
    if (!GetU32(framed, &pos, &crc)) return false;
    const std::string payload = framed.substr(pos);
    if (Crc32(payload) != crc) return false;  // torn / corrupt → treat as nothing durable
    pos = 0;
    PersistentState s;
    if (!GetI32(payload, &pos, &s.currentTerm)) return false;
    if (!GetI32(payload, &pos, &s.votedFor)) return false;
    if (!GetI32(payload, &pos, &s.lastIncludedIndex)) return false;
    if (!GetI32(payload, &pos, &s.lastIncludedTerm)) return false;
    if (!GetStr(payload, &pos, &s.snapshot)) return false;
    std::uint32_t count = 0;
    if (!GetU32(payload, &pos, &count)) return false;
    s.log.reserve(count);
    for (std::uint32_t i = 0; i < count; ++i) {
        LogEntry e;
        if (!GetI32(payload, &pos, &e.term)) return false;
        if (!GetStr(payload, &pos, &e.op)) return false;
        if (!GetStr(payload, &pos, &e.key)) return false;
        if (!GetStr(payload, &pos, &e.value)) return false;
        s.log.push_back(std::move(e));
    }
    *out = std::move(s);
    return true;
}

}  // namespace detail

// --- in-memory (default) ----------------------------------------------------

// Holds the durable image in memory. crash()/restart() drop only the live Node, so this blob
// survives — exactly the guarantee stable storage gives, with zero I/O and full determinism.
class MemStorage : public Storage {
 public:
    void saveHardState(int currentTerm, int votedFor) override {
        state_.currentTerm = currentTerm;
        state_.votedFor = votedFor;
    }
    void saveLog(const std::vector<LogEntry>& log) override { state_.log = log; }
    void saveSnapshot(int lastIncludedIndex, int lastIncludedTerm, const std::string& snapshot,
                      const std::vector<LogEntry>& log) override {
        state_.lastIncludedIndex = lastIncludedIndex;
        state_.lastIncludedTerm = lastIncludedTerm;
        state_.snapshot = snapshot;
        state_.log = log;
    }
    PersistentState load() override { return state_; }

 private:
    PersistentState state_;
};

// --- on-disk (real fsync) ---------------------------------------------------

// Writes the whole durable image to one file, atomically: write temp → fsync temp → rename over
// the target → fsync the directory. rename(2) is atomic, so a crash leaves either the complete
// old file or the complete new one — never a torn mix. The CRC prefix additionally catches a
// half-written temp on the very first save (no prior file to fall back to). load() returns a
// default (fresh) state when the file is missing or fails CRC.
class FileStorage : public Storage {
 public:
    explicit FileStorage(std::filesystem::path path) : path_(std::move(path)) {
        std::error_code ec;
        std::filesystem::create_directories(path_.parent_path(), ec);
        cache_ = load();  // start from whatever is durable
    }

    void saveHardState(int currentTerm, int votedFor) override {
        cache_.currentTerm = currentTerm;
        cache_.votedFor = votedFor;
        flush();
    }
    void saveLog(const std::vector<LogEntry>& log) override {
        cache_.log = log;
        flush();
    }
    void saveSnapshot(int lastIncludedIndex, int lastIncludedTerm, const std::string& snapshot,
                      const std::vector<LogEntry>& log) override {
        cache_.lastIncludedIndex = lastIncludedIndex;
        cache_.lastIncludedTerm = lastIncludedTerm;
        cache_.snapshot = snapshot;
        cache_.log = log;
        flush();
    }
    PersistentState load() override {
        const std::string bytes = ReadWholeFile(path_);
        PersistentState s;
        if (detail::Deserialize(bytes, &s)) return s;
        return PersistentState{};  // missing / torn / corrupt ⇒ nothing durable yet
    }

 private:
    void flush() {
        const std::string bytes = detail::Serialize(cache_);
        const std::filesystem::path tmp = path_.string() + ".tmp";
        const int fd = ::open(tmp.string().c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) return;
        std::size_t written = 0;
        while (written < bytes.size()) {
            const ssize_t n = ::write(fd, bytes.data() + written, bytes.size() - written);
            if (n < 0) {
                if (errno == EINTR) continue;
                ::close(fd);
                return;
            }
            written += static_cast<std::size_t>(n);
        }
        ::fsync(fd);  // Darwin: no fdatasync; fsync flushes data+metadata
        ::close(fd);
        std::error_code ec;
        std::filesystem::rename(tmp, path_, ec);  // atomic swap
        SyncDir(path_.parent_path());             // make the rename itself durable
    }

    static std::string ReadWholeFile(const std::filesystem::path& path) {
        const int fd = ::open(path.string().c_str(), O_RDONLY);
        if (fd < 0) return {};
        std::string out;
        char buf[4096];
        ssize_t n;
        while ((n = ::read(fd, buf, sizeof(buf))) > 0) out.append(buf, static_cast<std::size_t>(n));
        ::close(fd);
        return out;
    }

    static void SyncDir(const std::filesystem::path& dir) {
        const int fd = ::open(dir.string().c_str(), O_RDONLY);
        if (fd < 0) return;
        ::fsync(fd);
        ::close(fd);
    }

    std::filesystem::path path_;
    PersistentState cache_;
};

}  // namespace raft_sim

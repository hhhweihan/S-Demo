#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "lsm/coding.h"
#include "lsm/crc32c.h"

// 崩溃安全的 WAL（write-ahead log）。这是整个 LSM 崩溃安全的地基：任何一次 Put/Delete 在返回成功
// 前，其变更先以一条 record 追加进 WAL 并 fsync 落盘，之后才写入内存 MemTable。进程/机器崩溃后重开
// 库，Replay() 把 WAL 里所有完整 record 回放进 MemTable，从而"已确认的写入不丢"。
//
// record 帧格式（全定长头，小端）：
//     length(4) | type(1) | crc32c(4) | payload(length 字节)
//   - length：payload 字节数。
//   - type：record 类型（这里只用 kFullType；分片式 record 是 LevelDB 为超大 value 跨 block
//   设计的，
//           mini 版不需要）。
//   - crc32c：对 **type + payload** 求校验和（覆盖 type 是为了 type 字节本身也被保护）。
//   - payload：上层（DB 层）自定义的不透明字节。WAL 不解释其内容。
//
// **崩溃尾部恢复**是关键：回放时对每条 record 重算 CRC。一旦遇到① 头部不完整、② payload
// 长度越过文件 尾（写到一半断电）、或③ CRC 不匹配（坏盘/撕裂写），立刻停止回放并丢弃该 record
// 及其后所有字节。 因为 record 是顺序追加的，第一条损坏 record
// 之后的内容要么不存在、要么无法确认完整，一律作废——这
// 正是"只丢崩溃瞬间那条没写完的、保住之前所有已确认写"的语义。
namespace mini_lsm {

// record 类型。当前只有"完整单条"。保留 1 字节为将来分片/校验点留扩展位。
enum class RecordType : std::uint8_t {
    kFullType = 1,
};

// header 固定 9 字节：length(4) + type(1) + crc32c(4)。
inline constexpr std::size_t kRecordHeaderSize = 9;

// WAL 追加写入器。持有一个以 O_APPEND 打开的持久 fd，AddRecord 追加一帧，Sync 落盘。
class RecordWriter {
 public:
    RecordWriter() = default;

    ~RecordWriter() { Close(); }

    RecordWriter(const RecordWriter&) = delete;
    RecordWriter& operator=(const RecordWriter&) = delete;

    // 打开（或创建）WAL 文件用于追加。已存在则续写（恢复后继续用同一 WAL 的场景）。
    bool Open(const std::string& path) {
        Close();
        // O_APPEND：每次 write 原子地追加到文件尾，避免并发/偏移错乱。0644 常规权限。
        fd_ = ::open(path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        return fd_ >= 0;
    }

    bool is_open() const { return fd_ >= 0; }

    // 追加一条 record。payload 为上层不透明字节。返回是否全部写出（不含落盘，落盘靠 Sync）。
    bool AddRecord(const std::string& payload) {
        if (fd_ < 0) {
            return false;
        }
        const auto type = static_cast<std::uint8_t>(RecordType::kFullType);

        // CRC 覆盖 type + payload：先拼出被校验区，再算 CRC。
        std::string crc_region;
        crc_region.reserve(1 + payload.size());
        crc_region.push_back(static_cast<char>(type));
        crc_region.append(payload);
        const std::uint32_t crc = crc32c::Value(crc_region);

        std::string frame;
        frame.reserve(kRecordHeaderSize + payload.size());
        PutFixed32(&frame, static_cast<std::uint32_t>(payload.size()));
        frame.push_back(static_cast<char>(type));
        PutFixed32(&frame, crc);
        frame.append(payload);

        return WriteAll(frame.data(), frame.size());
    }

    // 落盘。真持久性的关键调用——返回成功即数据到达稳定存储。
    bool Sync() {
        if (fd_ < 0) {
            return false;
        }
#if defined(__APPLE__)
        // macOS 上 fsync 只保证写进磁盘缓存，不保证落到盘片；F_FULLFSYNC 才是真正的 durable
        // barrier。 F_FULLFSYNC 在部分文件系统/外接盘上可能失败（返回非 0），此时退回 fsync
        // 尽力而为。
        if (::fcntl(fd_, F_FULLFSYNC) == 0) {
            return true;
        }
        return ::fsync(fd_) == 0;
#elif defined(__linux__)
        // Linux：fdatasync 只刷数据+必要的元数据（size），比 fsync 省一次 inode 时间戳写。
        return ::fdatasync(fd_) == 0;
#else
        return ::fsync(fd_) == 0;
#endif
    }

    void Close() {
        if (fd_ >= 0) {
            ::close(fd_);
            fd_ = -1;
        }
    }

 private:
    // 循环写直到写完或出错——单次 ::write 可能短写（尤其被信号打断）。
    bool WriteAll(const char* data, std::size_t n) {
        std::size_t written = 0;
        while (written < n) {
            const ssize_t r = ::write(fd_, data + written, n - written);
            if (r < 0) {
                if (errno == EINTR) {
                    continue;  // 被信号打断，重试
                }
                return false;
            }
            written += static_cast<std::size_t>(r);
        }
        return true;
    }

    int fd_ = -1;
};

// WAL 回放读取器。把整个文件读入内存后逐帧解析，遇首个损坏/截断 record 即停。
// 简化：一次性读全文件（mini 版 WAL 不会很大）；真 LevelDB 是流式按 32KB block 读。
class RecordReader {
 public:
    // 回放：返回按写入顺序排列的所有 **完整** record 的 payload 列表。
    // 文件不存在视作空 WAL（首次启动）。clean_out（可选）置为"是否读到文件真正末尾而无截断/损坏"。
    static std::vector<std::string> Replay(const std::string& path, bool* clean_out = nullptr) {
        std::vector<std::string> records;
        if (clean_out != nullptr) {
            *clean_out = true;
        }

        const int fd = ::open(path.c_str(), O_RDONLY);
        if (fd < 0) {
            return records;  // 无 WAL：当作空
        }
        std::string data = ReadAll(fd);
        ::close(fd);

        std::size_t pos = 0;
        while (true) {
            // ① 头部不完整 → 截断尾部。
            if (pos + kRecordHeaderSize > data.size()) {
                if (pos != data.size() && clean_out != nullptr) {
                    *clean_out = false;  // 有残留半个头
                }
                break;
            }
            const std::uint32_t len = DecodeFixed32(data.data() + pos);
            const auto type = static_cast<std::uint8_t>(data[pos + 4]);
            const std::uint32_t expected_crc = DecodeFixed32(data.data() + pos + 5);
            const std::size_t body = pos + kRecordHeaderSize;

            // ② payload 越过文件尾（写到一半断电）→ 截断尾部。
            if (body + len > data.size()) {
                if (clean_out != nullptr) {
                    *clean_out = false;
                }
                break;
            }

            // ③ 重算 CRC（覆盖 type + payload），不符 → 损坏尾部。
            std::string crc_region;
            crc_region.reserve(1 + len);
            crc_region.push_back(static_cast<char>(type));
            crc_region.append(data, body, len);
            if (crc32c::Value(crc_region) != expected_crc) {
                if (clean_out != nullptr) {
                    *clean_out = false;
                }
                break;
            }

            records.emplace_back(data.substr(body, len));
            pos = body + len;
        }
        return records;
    }

 private:
    static std::string ReadAll(int fd) {
        std::string out;
        char buf[4096];
        while (true) {
            const ssize_t r = ::read(fd, buf, sizeof(buf));
            if (r < 0) {
                if (errno == EINTR) {
                    continue;
                }
                break;  // 读错误：返回已读到的部分
            }
            if (r == 0) {
                break;  // EOF
            }
            out.append(buf, static_cast<std::size_t>(r));
        }
        return out;
    }
};

}  // namespace mini_lsm

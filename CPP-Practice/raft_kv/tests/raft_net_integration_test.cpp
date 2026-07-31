// Real crash/recovery test for the network Raft server: fork+exec three actual `raft_kv_net_demo`
// OS processes, talk to them over real TCP, `kill(-SIGKILL)` the leader's real PID, and confirm
// the survivors elect a new leader and the restarted node recovers from its on-disk FileStorage.
//
// This is exactly what raft_sim.h's chaos tests cannot exercise: there, "crash" is a C++ method
// call that drops a struct's fields — real UB-free, deterministic, and unable to say anything
// about actual process teardown, socket half-close, or on-disk durability across a real reboot.
// SIGKILL bypasses every userspace cleanup path (no destructors, no atexit, no flushing anything
// not already fsync'd) — the strongest crash test available short of pulling power.
//
// Real wall-clock, real process scheduling: slower and nondeterministic vs. the simulation, so
// this file is tagged with the CTest `integration` label and excluded from the default run (see
// CMakeLists.txt and .github/workflows/ci.yml). Run explicitly with:
//   ctest --test-dir build -L integration --output-on-failure

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#ifndef RAFT_KV_NET_DEMO_PATH
#error "RAFT_KV_NET_DEMO_PATH must be defined by CMake to the raft_kv_net_demo binary path"
#endif

namespace {

struct Status {
    bool ok = false;
    std::string role;
    int term = -1;
    int commit = -1;
    int applied = -1;
    int leader = -1;
};

// Sends one line over a fresh TCP connection and returns the single-line reply, or "" on any
// failure/timeout — callers poll, so a transient failure (e.g. connecting mid-election) is normal.
std::string TcpRequest(std::uint16_t port, const std::string& line, int timeoutMs = 500) {
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) return "";
    timeval tv{timeoutMs / 1000, (timeoutMs % 1000) * 1000};
    ::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    ::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != 0) {
        ::close(fd);
        return "";
    }
    std::string out = line + "\n";
    if (::send(fd, out.data(), out.size(), 0) < 0) {
        ::close(fd);
        return "";
    }
    char buf[4096];
    ssize_t n = ::recv(fd, buf, sizeof(buf), 0);
    ::close(fd);
    if (n <= 0) return "";
    std::string reply(buf, static_cast<std::size_t>(n));
    while (!reply.empty() && (reply.back() == '\n' || reply.back() == '\r')) reply.pop_back();
    return reply;
}

Status ParseStatus(const std::string& line) {
    // "ROLE=Leader TERM=2 COMMIT=1 APPLIED=1 LEADER=0"
    Status s;
    if (line.rfind("ROLE=", 0) != 0) return s;
    auto field = [&](const char* key) -> std::string {
        auto pos = line.find(key);
        if (pos == std::string::npos) return "";
        pos += std::strlen(key);
        auto end = line.find(' ', pos);
        return line.substr(pos, end == std::string::npos ? std::string::npos : end - pos);
    };
    s.role = field("ROLE=");
    s.term = std::atoi(field("TERM=").c_str());
    s.commit = std::atoi(field("COMMIT=").c_str());
    s.applied = std::atoi(field("APPLIED=").c_str());
    s.leader = std::atoi(field("LEADER=").c_str());
    s.ok = true;
    return s;
}

Status QueryStatus(std::uint16_t adminPort) { return ParseStatus(TcpRequest(adminPort, "STATUS")); }

// Polls `probe` every 50ms until it returns true or `budgetMs` elapses. Returns whether it held.
bool WaitFor(std::int64_t budgetMs, const std::function<bool()>& probe) {
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(budgetMs);
    do {
        if (probe()) return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } while (std::chrono::steady_clock::now() < deadline);
    return probe();
}

struct Node {
    int id;
    std::uint16_t peerPort;
    std::uint16_t adminPort;
    std::filesystem::path storageDir;
    pid_t pid = -1;
};

std::string PeerSpec(const std::vector<Node>& nodes, int selfId) {
    std::string spec;
    for (const auto& n : nodes) {
        if (n.id == selfId) continue;
        if (!spec.empty()) spec += ",";
        spec += std::to_string(n.id) + "=127.0.0.1:" + std::to_string(n.peerPort);
    }
    return spec;
}

pid_t SpawnNode(const Node& node, const std::vector<Node>& all) {
    pid_t pid = ::fork();
    if (pid < 0) throw std::runtime_error("fork failed");
    if (pid == 0) {
        // Child: quiet its stdout/stderr so ctest output stays readable, then exec the real binary.
        int devnull = ::open("/dev/null", O_WRONLY);
        if (devnull >= 0) {
            ::dup2(devnull, STDOUT_FILENO);
            ::dup2(devnull, STDERR_FILENO);
        }
        std::string idStr = std::to_string(node.id);
        std::string peers = PeerSpec(all, node.id);
        std::string peerPortStr = std::to_string(node.peerPort);
        std::string adminPortStr = std::to_string(node.adminPort);
        std::string dirStr = node.storageDir.string();
        ::execl(RAFT_KV_NET_DEMO_PATH, RAFT_KV_NET_DEMO_PATH, idStr.c_str(), peers.c_str(),
               peerPortStr.c_str(), adminPortStr.c_str(), dirStr.c_str(), static_cast<char*>(nullptr));
        _exit(127);  // execl only returns on failure
    }
    return pid;
}

}  // namespace

TEST(RaftNetIntegration, KillLeaderReelectsAndRestartRecovers) {
    ::signal(SIGPIPE, SIG_IGN);  // a peer closing mid-write must not kill the test process

    const auto base = std::filesystem::temp_directory_path() /
                      ("raft_net_it_" + std::to_string(::getpid()));
    std::filesystem::remove_all(base);

    std::vector<Node> nodes = {
        {0, 19301, 19401, base / "node0", -1},
        {1, 19302, 19402, base / "node1", -1},
        {2, 19303, 19403, base / "node2", -1},
    };
    for (auto& n : nodes) n.pid = SpawnNode(n, nodes);

    auto findLeader = [&]() -> int {
        for (auto& n : nodes) {
            Status s = QueryStatus(n.adminPort);
            if (s.ok && s.role == "Leader") return n.id;
        }
        return -1;
    };
    int leaderId = -1;
    ASSERT_TRUE(WaitFor(5000, [&] { return (leaderId = findLeader()) >= 0; }))
        << "no leader elected within budget";
    auto leaderIt = std::find_if(nodes.begin(), nodes.end(), [&](const Node& n) { return n.id == leaderId; });
    ASSERT_NE(leaderIt, nodes.end());
    int firstTerm = QueryStatus(leaderIt->adminPort).term;

    // Write through the leader and confirm it is actually committed and readable.
    ASSERT_TRUE(WaitFor(2000, [&] { return TcpRequest(leaderIt->adminPort, "PUT k1 v1").rfind("OK", 0) == 0; }));
    ASSERT_TRUE(WaitFor(2000, [&] { return TcpRequest(leaderIt->adminPort, "GET k1") == "VALUE v1"; }));

    // The strongest crash there is: SIGKILL bypasses every userspace cleanup path.
    pid_t killedPid = leaderIt->pid;
    ASSERT_EQ(::kill(killedPid, SIGKILL), 0);
    int waitStatus = 0;
    ASSERT_EQ(::waitpid(killedPid, &waitStatus, 0), killedPid);
    ASSERT_TRUE(WIFSIGNALED(waitStatus));
    ASSERT_EQ(WTERMSIG(waitStatus), SIGKILL);

    std::vector<Node> survivors;
    for (auto& n : nodes)
        if (n.id != leaderId) survivors.push_back(n);

    int newLeaderId = -1;
    ASSERT_TRUE(WaitFor(5000, [&] {
        for (auto& n : survivors) {
            Status s = QueryStatus(n.adminPort);
            if (s.ok && s.role == "Leader" && s.term > firstTerm) {
                newLeaderId = n.id;
                return true;
            }
        }
        return false;
    })) << "remaining majority failed to elect a new leader after SIGKILL";

    auto newLeaderIt =
        std::find_if(survivors.begin(), survivors.end(), [&](const Node& n) { return n.id == newLeaderId; });
    ASSERT_NE(newLeaderIt, survivors.end());
    ASSERT_TRUE(WaitFor(2000, [&] { return TcpRequest(newLeaderIt->adminPort, "GET k1") == "VALUE v1"; }))
        << "committed key did not survive leader failover";

    // Restart the killed node pointed at the SAME storage directory: recovery must come purely
    // from what FileStorage actually fsync'd, not from any in-process state (there is none left).
    auto killedNodeIt = std::find_if(nodes.begin(), nodes.end(), [&](const Node& n) { return n.id == leaderId; });
    killedNodeIt->pid = SpawnNode(*killedNodeIt, nodes);

    // Recovery check: the restarted node's term must reach the cluster's
    // current term (proves it durably recalled currentTerm/votedFor and rejoined, not just booted).
    Status clusterNow = QueryStatus(newLeaderIt->adminPort);
    ASSERT_TRUE(WaitFor(5000, [&] { return QueryStatus(killedNodeIt->adminPort).term >= clusterNow.term; }))
        << "restarted node never caught up to the cluster's current term";
    ASSERT_TRUE(WaitFor(3000, [&] { return QueryStatus(killedNodeIt->adminPort).applied >= 1; }))
        << "restarted node did not replay/replicate the committed key after rejoining";

    for (auto& n : nodes) {
        if (::kill(n.pid, SIGKILL) == 0) ::waitpid(n.pid, nullptr, 0);
    }
    std::filesystem::remove_all(base);
}

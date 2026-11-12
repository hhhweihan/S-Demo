// raft_kv_net_demo — one real Raft node as its own OS process.
//
// Usage:
//   raft_kv_net_demo <id> <peers> <peer-port> <admin-port> <storage-dir>
//
//   <peers>       comma-separated "id=host:port" for every OTHER node, e.g.
//                 "1=127.0.0.1:17002,2=127.0.0.1:17003"
//   <peer-port>   this node's TCP port for Raft-to-Raft RPC
//   <admin-port>  this node's TCP port for the text PUT/GET/STATUS protocol
//   <storage-dir> directory for this node's on-disk log/snapshot (FileStorage) — pointing two
//                 invocations at the same directory across a kill+restart is how crash recovery
//                 is exercised for real.
//
// Three terminals, three processes, one cluster:
//   ./raft_kv_net_demo 0 "1=127.0.0.1:17002,2=127.0.0.1:17003" 17001 18001 /tmp/raft0
//   ./raft_kv_net_demo 1 "0=127.0.0.1:17001,2=127.0.0.1:17003" 17002 18002 /tmp/raft1
//   ./raft_kv_net_demo 2 "0=127.0.0.1:17001,1=127.0.0.1:17002" 17003 18003 /tmp/raft2
// then from a fourth terminal: `printf 'STATUS\n' | nc 127.0.0.1 18001`, `printf 'PUT k v\n' | nc
// ...`, and `kill -9 <pid of the current leader>` to watch the survivors re-elect.

#include <unistd.h>

#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "net/raft_server.h"
#include "raft_storage.h"

namespace {

raft_net::RaftServer* g_server = nullptr;

void HandleSignal(int) {
    if (g_server) g_server->Stop();  // eventfd write is async-signal-safe
}

std::vector<raft_net::PeerAddr> ParsePeers(const std::string& spec) {
    std::vector<raft_net::PeerAddr> peers;
    std::stringstream ss(spec);
    std::string entry;
    while (std::getline(ss, entry, ',')) {
        if (entry.empty()) continue;
        auto eq = entry.find('=');
        auto colon = entry.find(':', eq);
        if (eq == std::string::npos || colon == std::string::npos) {
            std::fprintf(stderr, "bad peer spec: %s\n", entry.c_str());
            std::exit(2);
        }
        raft_net::PeerAddr addr;
        addr.id = std::atoi(entry.substr(0, eq).c_str());
        addr.host = entry.substr(eq + 1, colon - eq - 1);
        addr.port = static_cast<std::uint16_t>(std::atoi(entry.substr(colon + 1).c_str()));
        peers.push_back(addr);
    }
    return peers;
}

const char* RoleName(raft_sim::Role r) {
    switch (r) {
        case raft_sim::Role::Follower:
            return "Follower";
        case raft_sim::Role::Candidate:
            return "Candidate";
        case raft_sim::Role::Leader:
            return "Leader";
    }
    return "?";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 6) {
        std::fprintf(stderr, "usage: %s <id> <peers> <peer-port> <admin-port> <storage-dir>\n",
                     argv[0]);
        return 2;
    }

    raft_net::ServerConfig cfg;
    cfg.id = std::atoi(argv[1]);
    cfg.peers = ParsePeers(argv[2]);
    cfg.peerPort = static_cast<std::uint16_t>(std::atoi(argv[3]));
    cfg.adminPort = static_cast<std::uint16_t>(std::atoi(argv[4]));
    cfg.storage = std::make_shared<raft_sim::FileStorage>(
        std::filesystem::path(argv[5]) / ("node" + std::to_string(cfg.id) + ".raft"));

    raft_net::RaftServer server(std::move(cfg));
    g_server = &server;
    std::signal(SIGINT, HandleSignal);
    std::signal(SIGTERM, HandleSignal);

    int statusTimer = server.Loop().CreateTimer([&server] {
        std::printf("[node %d] role=%s term=%d commit=%d applied=%d leader=%d pid=%d\n",
                    server.id(), RoleName(server.role()), server.term(), server.commitIndex(),
                    server.lastApplied(), server.leaderHint(), static_cast<int>(::getpid()));
        std::fflush(stdout);
    });
    server.Loop().StartPeriodic(statusTimer, std::chrono::milliseconds(1000));

    server.Run();  // returns once Stop() is called (SIGINT/SIGTERM)
    return 0;
}

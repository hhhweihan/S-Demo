#include "raft_sim.h"

#include <algorithm>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <cstdint>

using raft_sim::Cluster;
using raft_sim::Config;
using raft_sim::FileStorage;
using raft_sim::Role;
using raft_sim::Storage;
using raft_sim::StorageFactory;

namespace {

// The invariants that must hold in EVERY reachable state (election safety, log matching,
// state-machine safety) are asserted inside Cluster::checkInvariants() after every step,
// so any run that survives to completion has already proven them. These tests drive the
// cluster through the failure scenarios that make those invariants non-trivial, and add
// the liveness checks that can only be stated as "eventually" properties.

// A healthy cluster must elect exactly one leader and keep it. Runs many seeds because
// election timing is randomized per seed; a flaky election rule would fail on some of them.
TEST(RaftChaos, ElectsSingleStableLeaderAcrossSeeds) {
    for (std::uint64_t seed = 1; seed <= 50; ++seed) {
        Cluster cluster(5, seed);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 2000))
            << "no leader elected, seed=" << seed;
        int leader = cluster.currentLeader();
        // Let it settle; a correct cluster does not churn leaders when the network is clean.
        cluster.runFor(1000);
        EXPECT_EQ(cluster.currentLeader(), leader)
            << "leader churned on a clean network, seed=" << seed;
    }
}

// Committed entries must replicate to a majority and be applied identically everywhere.
TEST(RaftChaos, CommitsClientWritesOnHealthyCluster) {
    for (std::uint64_t seed = 1; seed <= 20; ++seed) {
        Cluster cluster(5, seed);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 2000));

        for (int i = 0; i < 10; ++i) {
            int idx = cluster.submit("put", "k" + std::to_string(i), "v" + std::to_string(i));
            ASSERT_GE(idx, 0) << "no leader to accept write, seed=" << seed;
            ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 2000))
                << "write " << i << " never committed, seed=" << seed;
        }
        // Every applied value must be visible through the leader's state machine.
        int leader = cluster.currentLeader();
        for (int i = 0; i < 10; ++i)
            EXPECT_EQ(cluster.node(leader).store.get("k" + std::to_string(i)),
                      std::optional<std::string>("v" + std::to_string(i)));
    }
}

// Lossy + reordering + duplicating network: progress must still be safe (invariants) and,
// given enough time, live (writes eventually commit).
TEST(RaftChaos, MakesProgressUnderLossyNetwork) {
    for (std::uint64_t seed = 1; seed <= 20; ++seed) {
        Config cfg;
        cfg.dropRate = 0.15;
        cfg.dupRate = 0.10;
        cfg.delayMin = 1;
        cfg.delayMax = 20;  // wide jitter => frequent reordering
        Cluster cluster(5, seed, cfg);

        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 5000))
            << "no leader under loss, seed=" << seed;

        int committed = 0;
        for (int i = 0; i < 5; ++i) {
            int idx = cluster.submit("put", "k" + std::to_string(i), std::to_string(i));
            if (idx < 0) {  // leader may have just been lost; give it a beat and retry
                cluster.runFor(200);
                idx = cluster.submit("put", "k" + std::to_string(i), std::to_string(i));
            }
            if (idx >= 0 && cluster.runUntil([&] { return cluster.isCommitted(idx); }, 4000))
                ++committed;
        }
        EXPECT_GT(committed, 0) << "no write committed under loss, seed=" << seed;
    }
}

// Leader Completeness + Liveness across a partition: a write committed by the majority side
// must survive, and after healing the cluster must re-converge and accept new writes.
TEST(RaftChaos, SurvivesPartitionAndHeals) {
    for (std::uint64_t seed = 1; seed <= 20; ++seed) {
        Cluster cluster(5, seed);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000));

        // Isolate the leader with one follower (minority); the other three keep quorum.
        int leader = cluster.currentLeader();
        std::vector<int> minority{leader};
        std::vector<int> majority;
        for (int i = 0; i < cluster.size(); ++i)
            if (i != leader) {
                if (static_cast<int>(minority.size()) < 2)
                    minority.push_back(i);
                else
                    majority.push_back(i);
            }
        cluster.partition(minority, majority);

        // The majority side must elect a new leader and commit a write the old leader can't.
        ASSERT_TRUE(cluster.runUntil(
            [&] {
                int l = cluster.currentLeader();
                return l >= 0 && std::find(majority.begin(), majority.end(), l) != majority.end();
            },
            4000))
            << "majority did not elect a new leader, seed=" << seed;

        int idx = cluster.submit("put", "survivor", "yes");
        ASSERT_GE(idx, 0);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 4000))
            << "majority could not commit during partition, seed=" << seed;

        // Heal: the committed entry must survive everywhere and new writes must commit again.
        cluster.heal();
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 4000));
        int idx2 = cluster.submit("put", "after_heal", "ok");
        ASSERT_GE(idx2, 0);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx2); }, 4000))
            << "no progress after heal, seed=" << seed;

        // The pre-heal committed entry is part of the agreed committed prefix forever.
        EXPECT_TRUE(cluster.isCommitted(idx));
    }
}

// Crash the leader, let survivors re-elect and keep committing, then bring the old leader
// back — a 5-node cluster only tolerates two simultaneous failures, so we restart each
// crashed node before crashing the next to keep quorum (and exercise log recovery).
TEST(RaftChaos, ReelectsAfterLeaderCrashes) {
    for (std::uint64_t seed = 1; seed <= 20; ++seed) {
        Cluster cluster(5, seed);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000));

        for (int round = 0; round < 4; ++round) {
            int idx = cluster.submit("put", "r" + std::to_string(round), "x");
            ASSERT_GE(idx, 0);
            ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 3000))
                << "round " << round << " never committed, seed=" << seed;

            int leader = cluster.currentLeader();
            cluster.crash(leader);
            ASSERT_TRUE(cluster.runUntil(
                [&] {
                    int l = cluster.currentLeader();
                    return l >= 0 && l != leader;
                },
                4000))
                << "no re-election after crashing leader, seed=" << seed;
            cluster.restart(leader);  // restore quorum headroom before the next crash
            cluster.runFor(300);      // let the restarted node catch up
        }
    }
}

// ============================ productionization ============================
// These exercise the features layered on top of the core simulation: real on-disk
// persistence, snapshot/InstallSnapshot, ReadIndex linearizable reads, and Pre-Vote.

// A clean, unique temp directory per test, wiped on entry so re-runs start durable-empty.
std::filesystem::path freshDir(const std::string& tag) {
    std::filesystem::path dir = std::filesystem::temp_directory_path() / ("raft_sim_" + tag);
    std::error_code ec;
    std::filesystem::remove_all(dir, ec);
    std::filesystem::create_directories(dir, ec);
    return dir;
}

StorageFactory fileFactory(const std::filesystem::path& dir) {
    return [dir](int id) -> std::shared_ptr<Storage> {
        return std::make_shared<FileStorage>(dir / ("node" + std::to_string(id)) / "state.bin");
    };
}

// A committed write must survive a WHOLE-CLUSTER reboot that reads state back from disk — not
// from any in-memory retention. The second Cluster points its FileStorage at the same files,
// so every node reconstructs (term, votedFor, log) purely via Storage::load().
TEST(RaftProd, FileStorageSurvivesFullClusterReboot) {
    const std::filesystem::path dir = freshDir("reboot");
    const StorageFactory factory = fileFactory(dir);

    {
        Cluster cluster(3, /*seed=*/7, Config{}, factory);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000));
        for (int i = 0; i < 5; ++i) {
            int idx = cluster.submit("put", "k" + std::to_string(i), "v" + std::to_string(i));
            ASSERT_GE(idx, 0);
            ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 3000));
        }
    }

    // Brand-new cluster over the same on-disk state = power-cycle the whole cluster.
    Cluster rebooted(3, /*seed=*/99, Config{}, factory);
    // Persistent log must have come back from disk before any new election.
    bool anyLog = false;
    for (int i = 0; i < rebooted.size(); ++i) anyLog = anyLog || !rebooted.node(i).log.empty();
    EXPECT_TRUE(anyLog) << "no node reloaded its persistent log from disk";

    // After re-electing, a linearizable read must return the pre-reboot value: the write was
    // durable, and committing a fresh current-term entry re-applies the reloaded prefix.
    ASSERT_TRUE(rebooted.runUntil([&] { return rebooted.currentLeader() >= 0; }, 3000));
    int probe = rebooted.submit("put", "probe", "1");  // current-term entry unlocks commit
    ASSERT_GE(probe, 0);
    ASSERT_TRUE(rebooted.runUntil([&] { return rebooted.isCommitted(probe); }, 3000));
    int leader = rebooted.currentLeader();
    EXPECT_EQ(rebooted.linearizableGet(leader, "k0", 3000), std::optional<std::string>("v0"));
}

// Extend the crash/re-elect scenario to REAL disk: crash the leader, let survivors keep
// committing, then restart it — it must reload its term/log from FileStorage and rejoin without
// violating any invariant (checked continuously inside the cluster).
TEST(RaftProd, ReelectsAfterLeaderCrashesWithFileStorage) {
    for (std::uint64_t seed = 1; seed <= 8; ++seed) {
        const std::filesystem::path dir = freshDir("crash" + std::to_string(seed));
        Cluster cluster(5, seed, Config{}, fileFactory(dir));
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000));

        for (int round = 0; round < 3; ++round) {
            int idx = cluster.submit("put", "r" + std::to_string(round), "x");
            ASSERT_GE(idx, 0);
            ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 3000))
                << "round " << round << " never committed, seed=" << seed;

            int leader = cluster.currentLeader();
            cluster.crash(leader);
            ASSERT_TRUE(cluster.runUntil(
                [&] {
                    int l = cluster.currentLeader();
                    return l >= 0 && l != leader;
                },
                4000))
                << "no re-election after crashing leader, seed=" << seed;
            cluster.restart(leader);  // reload persistent state from disk
            cluster.runFor(300);
        }
    }
}

// A follower that falls far behind while the leader compacts its log must be caught up by
// InstallSnapshot — and the leader's log must stay bounded (compaction actually happens).
TEST(RaftProd, InstallSnapshotCatchesUpLaggingFollower) {
    for (std::uint64_t seed = 1; seed <= 10; ++seed) {
        Config cfg;
        cfg.snapshotThreshold = 5;  // compact once 5+ applied entries pile up past the snapshot
        Cluster cluster(5, seed, cfg);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000));

        int leader = cluster.currentLeader();
        int follower = (leader + 1) % 5;

        // Commit a few, then take the follower down so the leader compacts past it.
        for (int i = 0; i < 6; ++i) {
            int idx = cluster.submit("put", "a" + std::to_string(i), std::to_string(i));
            ASSERT_GE(idx, 0);
            ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 3000));
        }
        cluster.crash(follower);
        for (int i = 0; i < 20; ++i) {
            int idx = cluster.submit("put", "b" + std::to_string(i), std::to_string(i));
            if (idx < 0) {
                cluster.runFor(200);
                continue;
            }
            cluster.runUntil([&] { return cluster.isCommitted(idx); }, 3000);
        }

        leader = cluster.currentLeader();
        ASSERT_GE(leader, 0);
        // Compaction happened and bounded the log (unbounded would be ~27+ entries).
        EXPECT_GT(cluster.node(leader).lastIncludedIndex, 0) << "leader never snapshotted";
        EXPECT_LT(cluster.node(leader).log.size(), 20u) << "log not bounded by compaction";

        // Restart the follower; the leader must ship it a snapshot to catch it up.
        cluster.restart(follower);
        ASSERT_TRUE(
            cluster.runUntil([&] { return cluster.node(follower).lastIncludedIndex > 0; }, 5000))
            << "lagging follower was not caught up via InstallSnapshot, seed=" << seed;
    }
}

// A linearizable read (ReadIndex) must reflect the latest committed write.
TEST(RaftProd, ReadIndexReturnsCommittedValue) {
    for (std::uint64_t seed = 1; seed <= 10; ++seed) {
        Cluster cluster(5, seed);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000));
        int idx = cluster.submit("put", "key", "committed");
        ASSERT_GE(idx, 0);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 3000));
        int leader = cluster.currentLeader();
        EXPECT_EQ(cluster.linearizableGet(leader, "key", 3000),
                  std::optional<std::string>("committed"))
            << "ReadIndex did not return the committed value, seed=" << seed;
    }
}

// A stale (partitioned) leader must NOT serve a linearizable read: it cannot confirm leadership
// with a heartbeat majority, so ReadIndex reports unavailable rather than returning old data.
TEST(RaftProd, StaleLeaderCannotServeLinearizableRead) {
    for (std::uint64_t seed = 1; seed <= 10; ++seed) {
        Cluster cluster(5, seed);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000));
        int idx = cluster.submit("put", "key", "v0");
        ASSERT_GE(idx, 0);
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.isCommitted(idx); }, 3000));

        // Trap the old leader in a 2-node minority; the other 3 keep quorum.
        int oldLeader = cluster.currentLeader();
        std::vector<int> minority{oldLeader};
        std::vector<int> majority;
        for (int i = 0; i < cluster.size(); ++i)
            if (i != oldLeader) {
                if (static_cast<int>(minority.size()) < 2)
                    minority.push_back(i);
                else
                    majority.push_back(i);
            }
        cluster.partition(minority, majority);

        // The old leader still believes it leads, but a ReadIndex cannot be confirmed by a
        // majority it can no longer reach ⇒ no stale read.
        EXPECT_EQ(cluster.linearizableGet(oldLeader, "key", 1500), std::nullopt)
            << "stale leader served a linearizable read, seed=" << seed;
    }
}

// Pre-Vote: a node isolated long enough to time out repeatedly must NOT, on rejoining, disrupt a
// healthy leader — it never wins pre-votes (peers hearing heartbeats reject them), so its term
// never climbs and the incumbent keeps leading at the same term.
TEST(RaftProd, PreVoteAvoidsDisruptionOnRejoin) {
    for (std::uint64_t seed = 1; seed <= 10; ++seed) {
        Config cfg;
        cfg.preVote = true;
        Cluster cluster(5, seed, cfg);
        // Liveness with Pre-Vote enabled: a leader is still elected on a clean network.
        ASSERT_TRUE(cluster.runUntil([&] { return cluster.currentLeader() >= 0; }, 3000))
            << "pre-vote broke liveness, seed=" << seed;
        cluster.runFor(500);
        int leader = cluster.currentLeader();
        int term0 = cluster.node(leader).currentTerm;

        // Isolate one follower; it repeatedly times out and pre-votes, but reaches no one.
        int victim = (leader + 1) % 5;
        std::vector<int> others;
        for (int i = 0; i < cluster.size(); ++i)
            if (i != victim) others.push_back(i);
        cluster.partition({victim}, others);
        cluster.runFor(2000);
        EXPECT_EQ(cluster.node(victim).currentTerm, term0)
            << "isolated node bumped its term despite pre-vote, seed=" << seed;
        EXPECT_EQ(cluster.currentLeader(), leader) << "leader disturbed during isolation";

        // Heal: the rejoining node must not force a new election — peers still hear the leader.
        cluster.heal();
        cluster.runFor(1000);
        EXPECT_EQ(cluster.currentLeader(), leader) << "rejoin disrupted the leader, seed=" << seed;
        EXPECT_EQ(cluster.node(leader).currentTerm, term0)
            << "term climbed on rejoin despite pre-vote, seed=" << seed;
    }
}

}  // namespace

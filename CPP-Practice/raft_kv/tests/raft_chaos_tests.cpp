#include "raft_sim.h"

#include <algorithm>
#include <optional>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <cstdint>

using raft_sim::Cluster;
using raft_sim::Config;
using raft_sim::Role;

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

}  // namespace

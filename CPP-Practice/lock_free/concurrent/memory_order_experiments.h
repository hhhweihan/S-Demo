#pragma once

#include <atomic>
#include <cstddef>
#include <thread>

struct PublicationExperimentResult {
    std::size_t relaxed_mismatches = 0;
    std::size_t release_acquire_mismatches = 0;
    std::size_t iterations = 0;
};

namespace memory_order_experiments {

inline std::size_t run_publication_loop(std::size_t iterations,
                                        std::memory_order ready_store_order,
                                        std::memory_order ready_load_order) {
    std::size_t mismatches = 0;

    for (std::size_t iteration = 0; iteration < iterations; ++iteration) {
        std::atomic<int> payload{0};
        std::atomic<int> ready{0};
        int observed = 0;

        std::thread producer([&] {
            payload.store(42, std::memory_order_relaxed);
            ready.store(1, ready_store_order);
        });

        std::thread consumer([&] {
            while (ready.load(ready_load_order) == 0) {
            }
            observed = payload.load(std::memory_order_relaxed);
        });

        producer.join();
        consumer.join();

        if (observed != 42) {
            ++mismatches;
        }
    }

    return mismatches;
}

inline PublicationExperimentResult run_publication_experiment(std::size_t iterations) {
    PublicationExperimentResult result;
    result.iterations = iterations;
    result.relaxed_mismatches =
        run_publication_loop(iterations, std::memory_order_relaxed, std::memory_order_relaxed);
    result.release_acquire_mismatches =
        run_publication_loop(iterations, std::memory_order_release, std::memory_order_acquire);
    return result;
}

}  // namespace memory_order_experiments

#pragma once

#include <atomic>
#include <cstdint>

struct ABAExperimentResult {
    bool plain_head_cas_succeeded = false;
    bool tagged_head_cas_succeeded = false;
    std::uint32_t final_plain_value = 0;
    std::uint32_t final_tagged_index = 0;
    std::uint32_t final_tagged_tag = 0;
};

namespace aba_demo {

inline std::uint64_t pack(std::uint32_t index, std::uint32_t tag) {
    return (static_cast<std::uint64_t>(tag) << 32U) | index;
}

inline std::uint32_t unpack_index(std::uint64_t value) {
    return static_cast<std::uint32_t>(value & 0xffffffffULL);
}

inline std::uint32_t unpack_tag(std::uint64_t value) {
    return static_cast<std::uint32_t>(value >> 32U);
}

inline ABAExperimentResult run_aba_compare_demo() {
    ABAExperimentResult result;

    std::atomic<std::uint32_t> plain_head{1};
    std::uint32_t plain_snapshot = plain_head.load(std::memory_order_acquire);

    plain_head.store(2, std::memory_order_release);
    plain_head.store(1, std::memory_order_release);

    result.plain_head_cas_succeeded = plain_head.compare_exchange_strong(
        plain_snapshot,
        3,
        std::memory_order_acq_rel,
        std::memory_order_acquire);
    result.final_plain_value = plain_head.load(std::memory_order_acquire);

    std::atomic<std::uint64_t> tagged_head{pack(1, 0)};
    std::uint64_t tagged_snapshot = tagged_head.load(std::memory_order_acquire);
    std::uint64_t expected = tagged_snapshot;

    tagged_head.store(pack(2, 1), std::memory_order_release);
    tagged_head.store(pack(1, 2), std::memory_order_release);

    result.tagged_head_cas_succeeded = tagged_head.compare_exchange_strong(
        expected,
        pack(3, unpack_tag(tagged_snapshot) + 1),
        std::memory_order_acq_rel,
        std::memory_order_acquire);

    const std::uint64_t tagged_final = tagged_head.load(std::memory_order_acquire);
    result.final_tagged_index = unpack_index(tagged_final);
    result.final_tagged_tag = unpack_tag(tagged_final);
    return result;
}

}  // namespace aba_demo

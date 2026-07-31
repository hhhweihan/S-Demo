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

// 把 (index, tag) 挤进一个 64 位字，这样一条 CAS 能同时比较指针和版本号——ABA 防护的核心：
// 即便 index 兜了一圈回到旧值，tag 已递增，CAS 仍会失败。
inline std::uint64_t pack(std::uint32_t index, std::uint32_t tag) {
    return (static_cast<std::uint64_t>(tag) << 32U) | index;
}

inline std::uint32_t unpack_index(std::uint64_t value) {
    return static_cast<std::uint32_t>(value & 0xffffffffULL);
}

inline std::uint32_t unpack_tag(std::uint64_t value) {
    return static_cast<std::uint32_t>(value >> 32U);
}

// 对照实验：同一个 A->B->A 序列下，裸指针 CAS 被骗成功，带 tag 的 CAS 检测到变化而失败。
inline ABAExperimentResult run_aba_compare_demo() {
    ABAExperimentResult result;

    std::atomic<std::uint32_t> plain_head{1};
    std::uint32_t plain_snapshot = plain_head.load(std::memory_order_acquire);

    // 制造经典 ABA：值离开 1 又回到 1，中间的变化对只看数值的 CAS 不可见。
    plain_head.store(2, std::memory_order_release);
    plain_head.store(1, std::memory_order_release);

    // 旧快照仍等于当前值，CAS 会误判"没人动过"而成功——这就是 ABA 漏洞。
    result.plain_head_cas_succeeded = plain_head.compare_exchange_strong(
        plain_snapshot, 3, std::memory_order_acq_rel, std::memory_order_acquire);
    result.final_plain_value = plain_head.load(std::memory_order_acquire);

    std::atomic<std::uint64_t> tagged_head{pack(1, 0)};
    std::uint64_t tagged_snapshot = tagged_head.load(std::memory_order_acquire);
    std::uint64_t expected = tagged_snapshot;

    // 同样让 index 回到 1，但每次写入都递增 tag，破坏"值相同"的假象。
    tagged_head.store(pack(2, 1), std::memory_order_release);
    tagged_head.store(pack(1, 2), std::memory_order_release);

    // 期望值带着旧 tag，而当前 tag 已变，CAS 必然失败——ABA 被挡住。
    result.tagged_head_cas_succeeded =
        tagged_head.compare_exchange_strong(expected, pack(3, unpack_tag(tagged_snapshot) + 1),
                                            std::memory_order_acq_rel, std::memory_order_acquire);

    const std::uint64_t tagged_final = tagged_head.load(std::memory_order_acquire);
    result.final_tagged_index = unpack_index(tagged_final);
    result.final_tagged_tag = unpack_tag(tagged_final);
    return result;
}

}  // namespace aba_demo

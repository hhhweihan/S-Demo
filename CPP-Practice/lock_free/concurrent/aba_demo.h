#pragma once  // 防止头文件重复包含

#include <atomic>  // 使用原子变量演示 CAS
#include <cstdint>  // 使用固定宽度整数

struct ABAExperimentResult {  // ABA 对比实验结果
    bool plain_head_cas_succeeded = false;  // 普通头指针 CAS 是否成功
    bool tagged_head_cas_succeeded = false;  // 带标签头指针 CAS 是否成功
    std::uint32_t final_plain_value = 0;  // 普通头指针最终值
    std::uint32_t final_tagged_index = 0;  // 带标签头指针最终索引
    std::uint32_t final_tagged_tag = 0;  // 带标签头指针最终版本
};  // 结束结果结构体

namespace aba_demo {  // ABA 演示命名空间

inline std::uint64_t pack(std::uint32_t index, std::uint32_t tag) {  // 打包索引和版本号
    return (static_cast<std::uint64_t>(tag) << 32U) | index;  // 高 32 位存标签低 32 位存索引
}  // 结束 pack

inline std::uint32_t unpack_index(std::uint64_t value) {  // 解包索引字段
    return static_cast<std::uint32_t>(value & 0xffffffffULL);  // 读取低 32 位
}  // 结束 unpack_index

inline std::uint32_t unpack_tag(std::uint64_t value) {  // 解包版本字段
    return static_cast<std::uint32_t>(value >> 32U);  // 读取高 32 位
}  // 结束 unpack_tag

inline ABAExperimentResult run_aba_compare_demo() {  // 运行普通 CAS 与带标签 CAS 对比
    ABAExperimentResult result;  // 保存实验结果

    std::atomic<std::uint32_t> plain_head{1};  // 普通头索引从 1 开始
    std::uint32_t plain_snapshot = plain_head.load(std::memory_order_acquire);  // 记录普通头快照

    plain_head.store(2, std::memory_order_release);  // 模拟头指针跳到其他节点
    plain_head.store(1, std::memory_order_release);  // 模拟头指针回到旧值

    result.plain_head_cas_succeeded = plain_head.compare_exchange_strong(  // 尝试用旧快照执行 CAS
        plain_snapshot,  // 期望值仍是旧头
        3,  // 新值写成 3
        std::memory_order_acq_rel,  // 成功时使用获取释放语义
        std::memory_order_acquire);  // 失败时使用获取语义
    result.final_plain_value = plain_head.load(std::memory_order_acquire);  // 记录普通头最终值

    std::atomic<std::uint64_t> tagged_head{pack(1, 0)};  // 带版本标签的头指针
    std::uint64_t tagged_snapshot = tagged_head.load(std::memory_order_acquire);  // 记录带标签快照
    std::uint64_t expected = tagged_snapshot;  // CAS 期望值副本

    tagged_head.store(pack(2, 1), std::memory_order_release);  // 索引变化并递增标签
    tagged_head.store(pack(1, 2), std::memory_order_release);  // 索引回到旧值但标签继续变化

    result.tagged_head_cas_succeeded = tagged_head.compare_exchange_strong(  // 带标签快照尝试 CAS
        expected,  // 期望完整的索引和标签都匹配
        pack(3, unpack_tag(tagged_snapshot) + 1),  // 准备写入新索引和新标签
        std::memory_order_acq_rel,  // 成功时使用获取释放语义
        std::memory_order_acquire);  // 失败时使用获取语义

    const std::uint64_t tagged_final = tagged_head.load(std::memory_order_acquire);  // 读取带标签最终头
    result.final_tagged_index = unpack_index(tagged_final);  // 保存最终索引
    result.final_tagged_tag = unpack_tag(tagged_final);  // 保存最终标签
    return result;  // 返回实验结果
}  // 结束 run_aba_compare_demo

}  // 结束 aba_demo 命名空间

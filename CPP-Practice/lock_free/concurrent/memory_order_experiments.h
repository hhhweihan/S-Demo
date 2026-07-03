#pragma once  // 防止头文件重复包含

#include <atomic>  // 使用原子变量演示内存序
#include <cstddef>  // 使用 std::size_t
#include <thread>  // 使用线程运行发布实验

struct PublicationExperimentResult {  // 发布实验统计结果
    std::size_t relaxed_mismatches = 0;  // relaxed 读写下的错配次数
    std::size_t release_acquire_mismatches = 0;  // release/acquire 下的错配次数
    std::size_t iterations = 0;  // 实验迭代次数
};  // 结束结果结构体

namespace memory_order_experiments {  // 内存序实验命名空间

inline std::size_t run_publication_loop(std::size_t iterations,  // 运行指定内存序的发布循环
                                        std::memory_order ready_store_order,  // ready 写入使用的内存序
                                        std::memory_order ready_load_order) {  // ready 读取使用的内存序
    std::size_t mismatches = 0;  // 统计观察到错误 payload 的次数

    for (std::size_t iteration = 0; iteration < iterations; ++iteration) {  // 多次重复实验
        std::atomic<int> payload{0};  // 被发布的数据
        std::atomic<int> ready{0};  // 发布完成标志
        int observed = 0;  // 消费者观察到的数据

        std::thread producer([&] {  // 生产者线程发布数据
            payload.store(42, std::memory_order_relaxed);  // 先写入负载数据
            ready.store(1, ready_store_order);  // 再按指定内存序发布 ready
        });  // 结束生产者线程创建

        std::thread consumer([&] {  // 消费者线程等待发布
            while (ready.load(ready_load_order) == 0) {  // 自旋等待 ready 标志
            }  // 结束自旋等待
            observed = payload.load(std::memory_order_relaxed);  // 读取发布后的负载
        });  // 结束消费者线程创建

        producer.join();  // 等待生产者结束
        consumer.join();  // 等待消费者结束

        if (observed != 42) {  // 检查是否观察到错误数据
            ++mismatches;  // 累加错配次数
        }  // 结束错配检查
    }  // 结束循环

    return mismatches;  // 返回错配总数
}  // 结束 run_publication_loop

inline PublicationExperimentResult run_publication_experiment(std::size_t iterations) {  // 运行两组发布实验
    PublicationExperimentResult result;  // 准备结果对象
    result.iterations = iterations;  // 记录迭代次数
    result.relaxed_mismatches =  // 统计 relaxed 组合结果
        run_publication_loop(iterations, std::memory_order_relaxed, std::memory_order_relaxed);  // 执行 relaxed 实验
    result.release_acquire_mismatches =  // 统计 release/acquire 组合结果
        run_publication_loop(iterations, std::memory_order_release, std::memory_order_acquire);  // 执行同步实验
    return result;  // 返回实验统计
}  // 结束 run_publication_experiment

}  // 结束 memory_order_experiments 命名空间

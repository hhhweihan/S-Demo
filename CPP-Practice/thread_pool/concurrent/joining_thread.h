#pragma once  // 保证头文件只被包含一次

#include <thread>  // 引入 std::thread
#include <utility>  // 引入移动和转发工具

class JoiningThread {  // 声明自动 join 的线程包装器
public:  // 对外公开线程包装接口
    JoiningThread() noexcept = default;  // 默认构造空线程

    template <typename F, typename... Args>  // 接受任意可调用对象和参数
    explicit JoiningThread(F&& func, Args&&... args)  // 构造并启动线程
        : thread_(std::forward<F>(func), std::forward<Args>(args)...) {}  // 完美转发到 std::thread

    JoiningThread(JoiningThread&& other) noexcept  // 移动构造线程所有权
        : thread_(std::move(other.thread_)) {}  // 接管底层线程

    JoiningThread& operator=(JoiningThread&& other) noexcept {  // 移动赋值线程所有权
        if (this != &other) {  // 避免自移动赋值
            join_if_needed();  // 先回收当前线程
            thread_ = std::move(other.thread_);  // 接管新线程
        }  // 结束自赋值检查
        return *this;  // 返回当前对象
    }  // 结束移动赋值

    JoiningThread(const JoiningThread&) = delete;  // 禁止复制线程所有权
    JoiningThread& operator=(const JoiningThread&) = delete;  // 禁止复制赋值线程所有权

    ~JoiningThread() {  // 析构时回收线程
        join_if_needed();  // 自动 join 可连接线程
    }  // 结束析构

    bool joinable() const noexcept {  // 查询底层线程是否可 join
        return thread_.joinable();  // 返回可连接状态
    }  // 结束 joinable

    void join() {  // 主动等待线程结束
        thread_.join();  // 调用底层 join
    }  // 结束 join

    std::thread::id get_id() const noexcept {  // 获取线程标识
        return thread_.get_id();  // 返回底层线程 id
    }  // 结束 get_id

private:  // 内部线程管理实现
    void join_if_needed() noexcept {  // 在需要时回收线程
        if (thread_.joinable()) {  // 仅 join 可连接线程
            thread_.join();  // 等待线程结束
        }  // 结束可连接检查
    }  // 结束 join_if_needed

    std::thread thread_;  // 持有底层线程对象
};  // 结束 JoiningThread 定义
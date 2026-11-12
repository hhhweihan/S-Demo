#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>

// 用 RAII 封装 FILE*：构造即打开，析构即关闭，异常路径下也不漏句柄。
class FileGuard {
    FILE* file_ = nullptr;  // nullptr 表示当前不持有资源（含被移走后的状态）。
    std::string path_;

    // 抽出关闭逻辑，供析构和移动赋值复用；置空避免重复 fclose。
    void close_current() noexcept {
        if (file_ != nullptr) {
            std::fclose(file_);
            file_ = nullptr;
        }
    }

 public:
    FileGuard(const char* path, const char* mode)
        : path_(path) {  // 先存 path，fopen 失败时用于异常信息。
        file_ = std::fopen(path, mode);
        if (file_ == nullptr) {
            // 构造失败即抛异常：保证对象一旦建成必然持有有效句柄（RAII 不变量）。
            throw std::runtime_error("failed to open file: " + path_);
        }
    }

    ~FileGuard() noexcept { close_current(); }

    // 禁拷贝：两个副本会各自 fclose 同一个 FILE*。
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    FileGuard(FileGuard&& other) noexcept : file_(other.file_), path_(std::move(other.path_)) {
        other.file_ = nullptr;  // 置空源，否则其析构会关闭已转移的句柄。
    }

    FileGuard& operator=(FileGuard&& other) noexcept {
        if (this != &other) {  // 自移动保护：否则会先关掉自己要接管的句柄。
            close_current();   // 先释放当前句柄再接管，避免泄漏。
            file_ = other.file_;
            path_ = std::move(other.path_);
            other.file_ = nullptr;
        }
        return *this;
    }

    FILE* get() const noexcept { return file_; }

    const std::string& path() const noexcept { return path_; }

    // 让 if (guard) 可用，判断是否持有有效句柄；explicit 避免误参与算术转换。
    explicit operator bool() const noexcept { return file_ != nullptr; }
};

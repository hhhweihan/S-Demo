#pragma once  // 防止头文件在同一编译单元中被重复包含。

#include <cstdio>  // 使用 C 文件接口 FILE、fopen、fclose。
#include <stdexcept>  // 使用 std::runtime_error 报告打开文件失败。
#include <string>  // 使用 std::string 保存文件路径。
#include <utility>  // 使用 std::move 实现移动构造和移动赋值。

class FileGuard {  // FileGuard 用 RAII 封装 FILE* 的打开、关闭和所有权转移。
  FILE* file_ = nullptr;  // 保存当前拥有的 C 文件句柄，nullptr 表示没有资源。
  std::string path_;  // 保存文件路径，便于错误信息和示例输出使用。

  void close_current() noexcept {  // 关闭当前持有的文件句柄，供析构和移动赋值复用。
    if (file_ != nullptr) {  // 只有确实持有文件时才调用 fclose。
      std::fclose(file_);  // 释放底层 C 文件资源。
      file_ = nullptr;  // 清空指针，避免悬空指针和重复关闭。
    }  // 条件块结束，nullptr 时无需处理。
  }  // 当前文件关闭逻辑结束。

public:  // 对外提供构造、析构、移动和访问接口。
  FileGuard(const char* path, const char* mode)  // 构造时接收文件路径和打开模式。
      : path_(path) {  // 先保存路径，后续失败时可用于异常信息。
    file_ = std::fopen(path, mode);  // 调用 C 标准库打开文件并取得 FILE*。
    if (file_ == nullptr) {  // fopen 失败时返回 nullptr。
      throw std::runtime_error("failed to open file: " + path_);  // 用异常向调用者报告资源获取失败。
    }  // 文件打开成功时继续持有资源。
  }  // 构造完成后对象拥有文件句柄。

  ~FileGuard() noexcept {  // 析构时自动释放文件资源。
    close_current();  // 关闭当前仍持有的文件句柄。
  }  // 析构结束后不再持有资源。

  FileGuard(const FileGuard&) = delete;  // 禁止拷贝构造，避免两个对象同时拥有同一个 FILE*。
  FileGuard& operator=(const FileGuard&) = delete;  // 禁止拷贝赋值，保持文件句柄的唯一所有权。

  FileGuard(FileGuard&& other) noexcept  // 移动构造把文件所有权从 other 转移到当前对象。
      : file_(other.file_),  // 接管源对象保存的 FILE*。
        path_(std::move(other.path_)) {  // 移动路径字符串，避免不必要拷贝。
    other.file_ = nullptr;  // 清空源对象句柄，防止源对象析构时关闭已转移资源。
  }  // 移动构造结束。

  FileGuard& operator=(FileGuard&& other) noexcept {  // 移动赋值把 other 的文件所有权转移到当前对象。
    if (this != &other) {  // 防止自移动赋值导致资源被错误清空。
      close_current();  // 先释放当前已有文件，避免资源泄漏。
      file_ = other.file_;  // 接管源对象的 FILE*。
      path_ = std::move(other.path_);  // 移动源对象的路径字符串。
      other.file_ = nullptr;  // 清空源对象句柄，使其析构时不再关闭该文件。
    }  // 非自赋值处理结束。
    return *this;  // 返回当前对象以支持赋值表达式链式使用。
  }  // 移动赋值结束。

  FILE* get() const noexcept {  // 暴露底层 FILE*，便于调用 fputs 等 C 文件函数。
    return file_;  // 返回当前持有的文件句柄。
  }  // 句柄访问结束。

  const std::string& path() const noexcept {  // 返回文件路径的只读引用。
    return path_;  // 避免复制字符串。
  }  // 路径访问结束。

  explicit operator bool() const noexcept {  // 支持用 if (file_guard) 判断是否持有有效文件。
    return file_ != nullptr;  // 非空句柄表示对象当前有效。
  }  // bool 转换结束。
};  // FileGuard 定义结束。
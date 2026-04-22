#pragma once

#include <cstdio>
#include <stdexcept>
#include <string>
#include <utility>

class FileGuard {
  FILE* file_ = nullptr;
  std::string path_;

  void close_current() noexcept {
    if (file_ != nullptr) {
      std::fclose(file_);
      file_ = nullptr;
    }
  }

public:
  FileGuard(const char* path, const char* mode)
      : path_(path) {
    file_ = std::fopen(path, mode);
    if (file_ == nullptr) {
      throw std::runtime_error("failed to open file: " + path_);
    }
  }

  ~FileGuard() noexcept {
    close_current();
  }

  FileGuard(const FileGuard&) = delete;
  FileGuard& operator=(const FileGuard&) = delete;

  FileGuard(FileGuard&& other) noexcept
      : file_(other.file_),
        path_(std::move(other.path_)) {
    other.file_ = nullptr;
  }

  FileGuard& operator=(FileGuard&& other) noexcept {
    if (this != &other) {
      close_current();
      file_ = other.file_;
      path_ = std::move(other.path_);
      other.file_ = nullptr;
    }
    return *this;
  }

  FILE* get() const noexcept {
    return file_;
  }

  const std::string& path() const noexcept {
    return path_;
  }

  explicit operator bool() const noexcept {
    return file_ != nullptr;
  }
};
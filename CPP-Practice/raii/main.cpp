#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "file_guard.h"
#include "scope_guard.h"

namespace {

void test_defer_macro() {
  std::cout << "[defer] begin\n";
  const std::string path = "defer_output.txt";
  FILE* file = std::fopen(path.c_str(), "w");
  if (file == nullptr) {
    throw std::runtime_error("failed to open defer output");
  }

  {
    DEFER(std::fclose(file));
    std::fputs("defer works\n", file);
    std::cout << "registered deferred close\n";
  }

  std::cout << "[defer] file closed via guard\n";
}

void test_file_guard_success_and_move() {
  std::cout << "[file_guard] begin\n";
  const std::string path = "file_guard_output.txt";

  FileGuard file(path.c_str(), "w");
  std::fputs("hello from FileGuard\n", file.get());
  std::cout << "opened: " << file.path() << "\n";

  FileGuard moved(std::move(file));
  std::cout << "after move, source valid=" << (file ? "true" : "false") << "\n";
  std::cout << "after move, target valid=" << (moved ? "true" : "false") << "\n";
  std::fputs("moved still owns file\n", moved.get());
}

void test_file_guard_open_failure() {
  std::cout << "[file_guard_fail] begin\n";

  try {
    FileGuard missing("missing_dir/not_exists.txt", "r");
    std::cout << "unexpected success: " << missing.path() << "\n";
  } catch (const std::exception& ex) {
    std::cout << "caught: " << ex.what() << "\n";
  }
}

}  // namespace

int main() {
  test_defer_macro();
  std::cout << "----\n";

  test_file_guard_success_and_move();
  std::cout << "----\n";

  test_file_guard_open_failure();
  return 0;
}
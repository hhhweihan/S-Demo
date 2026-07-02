#include <iostream>  // 使用 std::cout 输出演示信息。
#include <stdexcept>  // 捕获 std::exception 并抛出运行时错误。
#include <string>  // 使用 std::string 保存示例文件路径。
#include <utility>  // 使用 std::move 演示移动构造。

#include "file_guard.h"  // 引入 FileGuard，演示文件句柄的 RAII 管理。
#include "scope_guard.h"  // 引入 ScopeGuard 和 DEFER，演示作用域退出清理。

namespace {  // 匿名命名空间限制测试函数只在当前翻译单元可见。

void test_defer_macro() {  // 验证 DEFER 宏能在离开局部作用域时自动关闭文件。
  std::cout << "[defer] begin\n";  // 打印当前测试开始标记。
  const std::string path = "defer_output.txt";  // 定义临时输出文件路径。
  FILE* file = std::fopen(path.c_str(), "w");  // 使用 C 接口打开文件，准备写入。
  if (file == nullptr) {  // 检查 fopen 是否失败。
    throw std::runtime_error("failed to open defer output");  // 打开失败时抛出异常，避免后续使用空指针。
  }  // 文件打开成功后继续注册延迟清理。

  {  // 创建内部作用域，用来触发 DEFER 变量在块结束时析构。
    DEFER(std::fclose(file));  // 注册作用域退出时关闭 file 的清理动作。
    std::fputs("defer works\n", file);  // 向文件写入一行文本。
    std::cout << "registered deferred close\n";  // 提示延迟关闭已经注册。
  }  // 离开内部作用域，ScopeGuard 析构并调用 fclose。

  std::cout << "[defer] file closed via guard\n";  // 打印文件已由守卫关闭的结果信息。
}  // DEFER 宏测试结束。

void test_file_guard_success_and_move() {  // 验证 FileGuard 成功打开文件并支持移动构造。
  std::cout << "[file_guard] begin\n";  // 打印当前测试开始标记。
  const std::string path = "file_guard_output.txt";  // 定义 FileGuard 管理的输出文件路径。

  FileGuard file(path.c_str(), "w");  // 构造 FileGuard，打开文件并取得所有权。
  std::fputs("hello from FileGuard\n", file.get());  // 通过 get() 取得 FILE* 并写入文本。
  std::cout << "opened: " << file.path() << "\n";  // 输出当前打开的文件路径。

  FileGuard moved(std::move(file));  // 移动构造新对象，把文件所有权从 file 转移到 moved。
  std::cout << "after move, source valid=" << (file ? "true" : "false") << "\n";  // 输出移动后源对象是否仍持有文件。
  std::cout << "after move, target valid=" << (moved ? "true" : "false") << "\n";  // 输出移动后目标对象是否持有文件。
  std::fputs("moved still owns file\n", moved.get());  // 继续通过 moved 写入，证明所有权转移成功。
}  // FileGuard 成功路径和移动语义测试结束。

void test_file_guard_open_failure() {  // 验证 FileGuard 在文件打开失败时会抛出异常。
  std::cout << "[file_guard_fail] begin\n";  // 打印当前测试开始标记。

  try {  // 捕获预期中的打开失败异常。
    FileGuard missing("missing_dir/not_exists.txt", "r");  // 尝试读取不存在目录下的文件，预期构造失败。
    std::cout << "unexpected success: " << missing.path() << "\n";  // 如果没有异常，输出异常路径提示。
  } catch (const std::exception& ex) {  // 捕获 FileGuard 构造函数抛出的标准异常。
    std::cout << "caught: " << ex.what() << "\n";  // 输出异常信息，说明失败路径被正确处理。
  }  // try/catch 结束。
}  // FileGuard 失败路径测试结束。

}  // namespace  // 匿名命名空间结束。

int main() {  // 程序入口，依次运行 RAII 示例测试。
  test_defer_macro();  // 运行 DEFER 宏测试。
  std::cout << "----\n";  // 输出分隔线，便于阅读运行结果。

  test_file_guard_success_and_move();  // 运行 FileGuard 成功打开和移动语义测试。
  std::cout << "----\n";  // 输出分隔线，区分不同测试。

  test_file_guard_open_failure();  // 运行 FileGuard 打开失败异常路径测试。
  return 0;  // 返回 0 表示示例程序正常结束。
}  // main 函数结束。
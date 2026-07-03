#include <cstdlib>  // 使用 std::exit 在测试失败时终止程序，保持示例失败时立即停下。
#include <iostream>  // 使用 std::cout/std::cerr 输出示例结果。
#include <map>  // 使用 std::map 保存命令名到处理函数的映射。
#include <optional>  // 使用 std::bad_optional_access 验证 Optional 空值访问异常。
#include <sstream>  // 使用 std::ostringstream 收集输出，便于断言。
#include <string>  // 使用 std::string 作为 Optional、Variant 和命令参数类型。
#include <utility>  // 使用移动语义相关工具。

#include "templates/my_function.h"  // 引入手写 Function，用于擦除 lambda/函数指针类型。
#include "templates/my_optional.h"  // 引入手写 Optional，用于表示可选解析结果。
#include "templates/my_variant.h"  // 引入手写 Variant，用于保存多种命令参数类型。
#include "templates/my_tuple.h"  // 引入 Week10 手写 Tuple，用于保存命令条目。

namespace demo {  // 示例代码放在 demo 命名空间中，避免污染全局命名空间。

using erasure::Function;  // 引入手写 Function 类型，减少后续限定名。
using erasure::Optional;  // 引入手写 Optional 类型，减少后续限定名。
using erasure::Variant;  // 引入手写 Variant 类型，减少后续限定名。
using erasure::visit;  // 引入手写 visit 函数，用于访问 Variant 当前值。
using tuple::Tuple;  // 引入手写 Tuple 类型，用于组合命令名和处理器。
using tuple::get;  // 引入 Tuple 的 get 函数，用于按下标访问命令条目。

void expect(bool condition, const std::string& message) {  // 简单测试断言函数。
  if (!condition) {  // 条件不成立表示当前示例失败。
    std::cerr << "[FAIL] " << message << '\n';  // 输出失败信息到标准错误流。
    std::exit(1);  // 立即退出程序，避免继续运行产生误导输出。
  }  // 失败分支结束。
  std::cout << "[PASS] " << message << '\n';  // 条件成立时输出通过信息。
}  // expect 函数结束。

int add(int left, int right) {  // 普通函数，用于验证 Function 可以保存函数指针。
  return left + right;  // 返回两个整数之和。
}  // add 函数结束。

void run_optional_examples() {  // 运行手写 Optional 的构造、访问、异常和拷贝示例。
  std::cout << "== Optional examples ==\n";  // 输出当前示例分组标题。

  Optional<std::string> empty;  // 默认构造空 Optional，不保存 string。
  expect(!empty.has_value(), "default Optional has no value");  // 验证默认 Optional 没有值。
  expect(empty.value_or("fallback") == "fallback", "value_or returns fallback when empty");  // 验证空 Optional 返回默认值。

  bool threw = false;  // 记录空值访问是否抛出异常。
  try {  // 捕获 empty.value() 的预期异常。
    (void)empty.value();  // 访问空 Optional，预期抛出 bad_optional_access。
  } catch (const std::bad_optional_access&) {  // 捕获标准 optional 空值访问异常。
    threw = true;  // 标记异常已发生。
  }  // try/catch 结束。
  expect(threw, "value() throws std::bad_optional_access when empty");  // 验证空值访问确实抛出异常。

  Optional<std::string> name("template");  // 从字符串值构造有值 Optional。
  expect(name.has_value(), "Optional constructed from value has value");  // 验证从值构造后 has_value 为 true。
  expect(name.value() == "template", "value() returns stored string");  // 验证 value() 返回内部字符串。

  Optional<std::string> copied = name;  // 拷贝构造另一个 Optional。
  name = std::string("traits");  // 修改原 Optional 的值。
  expect(copied.value() == "template" && name.value() == "traits",  // 验证拷贝对象和原对象各自拥有独立值。
         "Optional copy owns an independent value");  // 断言说明文本。
}  // Optional 示例结束。

void run_variant_examples() {  // 运行手写 Variant 的构造、访问、赋值和 visit 示例。
  std::cout << "== Variant examples ==\n";  // 输出当前示例分组标题。

  Variant<int, std::string> value(42);  // 构造保存 int alternative 的 Variant。
  expect(value.index() == 0, "Variant<int, string>(42) stores alternative 0");  // 验证 int 是第 0 个 alternative。
  expect(value.get<int>() == 42, "Variant get<int>() returns 42");  // 验证按类型访问 int 成功。

  const std::string printed_int = visit([](const auto& item) {  // 用泛型 lambda 访问当前 Variant 值。
    std::ostringstream output;  // 创建字符串输出流收集当前值。
    output << item;  // 把当前 alternative 写入输出流。
    return output.str();  // 返回格式化后的字符串。
  }, value);  // 对当前保存 int 的 Variant 调用 visit。
  expect(printed_int == "42", "visit dispatches to int alternative");  // 验证 visit 分发到了 int 分支。

  value = std::string("hello");  // 给 Variant 赋值 string，切换当前 alternative。
  expect(value.index() == 1, "assigning string switches to alternative 1");  // 验证 string 是第 1 个 alternative。

  const std::string printed_string = visit([](const auto& item) {  // 再次用泛型 lambda 访问当前值。
    std::ostringstream output;  // 创建字符串输出流收集当前值。
    output << item;  // 把当前 alternative 写入输出流。
    return output.str();  // 返回格式化后的字符串。
  }, value);  // 对当前保存 string 的 Variant 调用 visit。
  expect(printed_string == "hello", "visit dispatches to string alternative");  // 验证 visit 分发到了 string 分支。
}  // Variant 示例结束。

void run_function_examples() {  // 运行手写 Function 保存不同 callable 的示例。
  std::cout << "== Function examples ==\n";  // 输出当前示例分组标题。

  Function<int(int, int)> lambda = [](int left, int right) { return left * right; };  // 保存无状态 lambda。
  expect(lambda(3, 4) == 12, "Function stores a lambda");  // 验证 lambda 被正确调用。

  Function<int(int, int)> pointer = add;  // 保存普通函数指针。
  expect(pointer(3, 4) == 7, "Function stores a function pointer");  // 验证函数指针被正确调用。

  int base = 10;  // 准备捕获值，用于构造有状态 lambda。
  Function<int(int, int)> stateful = [base](int left, int right) {  // 保存捕获 base 的 lambda。
    return base + left + right;  // 使用捕获值和参数计算结果。
  };  // 有状态 lambda 构造结束。
  expect(stateful(1, 2) == 13, "Function stores a stateful lambda");  // 验证有状态 lambda 被正确调用。

  Function<int(int, int)> copied = stateful;  // 拷贝 Function，触发擦除对象 clone。
  expect(copied(2, 3) == 15, "Function copy clones the erased callable");  // 验证拷贝后的 callable 仍可独立调用。
}  // Function 示例结束。

using CommandArg = Variant<int, double, std::string>;  // 命令参数可以是 int、double 或 string。
using CommandHandler = Function<void(const std::string&, CommandArg)>;  // 命令处理器接收命令名和一个参数。
using CommandEntry = Tuple<std::string, CommandHandler>;  // 命令条目用 Tuple 保存命令名和处理器。

Optional<CommandArg> parse_argument(const std::string& token) {  // 把字符串 token 解析为可选命令参数。
  try {  // 先尝试按整数解析。
    std::size_t consumed = 0;  // 记录 stoi 消耗了多少字符。
    int int_value = std::stoi(token, &consumed);  // 尝试把 token 转成 int。
    if (consumed == token.size()) {  // 只有完整消耗字符串才算纯整数。
      return CommandArg(int_value);  // 返回保存 int 的 Variant。
    }  // 部分解析成功时继续尝试其他类型。
  } catch (...) {  // stoi 失败时忽略异常，继续尝试 double。
  }  // 整数解析结束。

  try {  // 再尝试按浮点数解析。
    std::size_t consumed = 0;  // 记录 stod 消耗了多少字符。
    double double_value = std::stod(token, &consumed);  // 尝试把 token 转成 double。
    if (consumed == token.size()) {  // 只有完整消耗字符串才算纯浮点数。
      return CommandArg(double_value);  // 返回保存 double 的 Variant。
    }  // 部分解析成功时继续尝试 string。
  } catch (...) {  // stod 失败时忽略异常，继续按字符串处理。
  }  // 浮点解析结束。

  if (!token.empty()) {  // 非空且不是数字时按字符串参数处理。
    return CommandArg(token);  // 返回保存 string 的 Variant。
  }  // 空字符串不生成参数。
  return Optional<CommandArg>();  // 返回空 Optional，表示解析失败或没有参数。
}  // parse_argument 结束。

void run_command_parser_example() {  // 运行 Optional、Variant、Function 和 Tuple 的整合示例。
  std::cout << "== command parser integration ==\n";  // 输出当前示例分组标题。

  std::ostringstream log;  // 用字符串流模拟命令执行日志。
  CommandEntry echo("echo", CommandHandler([&log](const std::string& name, CommandArg arg) {  // 构造 echo 命令条目。
    log << name << '=';  // 先输出命令名和等号。
    visit([&log](const auto& value) { log << value; }, arg);  // 访问 Variant 参数并输出实际值。
  }));  // echo 命令条目构造结束。

  CommandEntry square("square", CommandHandler([&log](const std::string& name, CommandArg arg) {  // 构造 square 命令条目。
    log << name << '=';  // 先输出命令名和等号。
    visit([&log](const auto& value) {  // 根据 Variant 当前参数类型执行不同逻辑。
      using T = std::decay_t<decltype(value)>;  // 去掉引用/cv，得到当前参数的实际值类型。
      if constexpr (std::is_arithmetic_v<T>) {  // 数值类型支持平方运算。
        log << value * value;  // 输出参数平方。
      } else {  // 非数值类型不支持平方。
        log << "unsupported";  // 输出不支持提示。
      }  // if constexpr 结束。
    }, arg);  // 对命令参数执行 visit。
  }));  // square 命令条目构造结束。

  std::map<std::string, CommandHandler> handlers;  // 保存命令名到命令处理器的映射表。
  handlers.emplace(get<0>(echo), get<1>(echo));  // 从 echo Tuple 中取出名称和处理器并插入 map。
  handlers.emplace(get<0>(square), get<1>(square));  // 从 square Tuple 中取出名称和处理器并插入 map。

  auto first_arg = parse_argument("7");  // 解析整数参数字符串。
  expect(first_arg.has_value(), "parse_argument recognizes int arguments");  // 验证整数参数解析成功。
  handlers.at("square")("square", first_arg.value());  // 调用 square 命令处理器处理整数参数。
  expect(log.str() == "square=49", "command parser dispatches numeric command");  // 验证数值命令分发和执行结果。

  log.str("");  // 清空字符串流内容，准备下一次命令测试。
  log.clear();  // 清除字符串流状态位，确保后续写入可用。
  auto second_arg = parse_argument("hello");  // 解析字符串参数。
  expect(second_arg.has_value(), "parse_argument recognizes string arguments");  // 验证字符串参数解析成功。
  handlers.at("echo")("echo", second_arg.value());  // 调用 echo 命令处理器处理字符串参数。
  expect(log.str() == "echo=hello", "command parser dispatches string command");  // 验证字符串命令分发和执行结果。
}  // 命令解析整合示例结束。

}  // namespace demo  // 示例命名空间结束。

int main() {  // 程序入口，按主题依次运行类型擦除示例。
  demo::run_optional_examples();  // 运行 Optional 示例。
  std::cout << "----\n";  // 输出分隔线，便于阅读不同示例分组。

  demo::run_variant_examples();  // 运行 Variant 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_function_examples();  // 运行 Function 示例。
  std::cout << "----\n";  // 输出分隔线。

  demo::run_command_parser_example();  // 运行命令解析整合示例。
  return 0;  // 返回 0 表示所有示例通过。
}  // main 函数结束。
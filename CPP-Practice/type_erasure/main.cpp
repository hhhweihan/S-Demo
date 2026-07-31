#include <cstdlib>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

#include "templates/my_function.h"
#include "templates/my_optional.h"
#include "templates/my_tuple.h"
#include "templates/my_variant.h"

namespace demo {

using erasure::Function;
using erasure::Optional;
using erasure::Variant;
using erasure::visit;
using tuple::get;
using tuple::Tuple;

void expect(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << '\n';
        std::exit(1);
    }
    std::cout << "[PASS] " << message << '\n';
}

int add(int left, int right) {
    return left + right;
}

void run_optional_examples() {
    std::cout << "== Optional examples ==\n";

    Optional<std::string> empty;
    expect(!empty.has_value(), "default Optional has no value");
    expect(empty.value_or("fallback") == "fallback", "value_or returns fallback when empty");

    bool threw = false;
    try {
        (void)empty.value();
    } catch (const std::bad_optional_access&) {
        threw = true;
    }
    expect(threw, "value() throws std::bad_optional_access when empty");

    Optional<std::string> name("template");
    expect(name.has_value(), "Optional constructed from value has value");
    expect(name.value() == "template", "value() returns stored string");

    Optional<std::string> copied = name;
    name = std::string("traits");
    // 改原对象后 copied 仍为旧值，验证拷贝是深拷贝、两者存储独立。
    expect(copied.value() == "template" && name.value() == "traits",
           "Optional copy owns an independent value");
}

void run_variant_examples() {
    std::cout << "== Variant examples ==\n";

    Variant<int, std::string> value(42);
    expect(value.index() == 0, "Variant<int, string>(42) stores alternative 0");
    expect(value.get<int>() == 42, "Variant get<int>() returns 42");

    const std::string printed_int = visit(
        [](const auto& item) {
            std::ostringstream output;
            output << item;
            return output.str();
        },
        value);
    expect(printed_int == "42", "visit dispatches to int alternative");

    value = std::string("hello");
    expect(value.index() == 1, "assigning string switches to alternative 1");

    const std::string printed_string = visit(
        [](const auto& item) {
            std::ostringstream output;
            output << item;
            return output.str();
        },
        value);
    expect(printed_string == "hello", "visit dispatches to string alternative");
}

void run_function_examples() {
    std::cout << "== Function examples ==\n";

    Function<int(int, int)> lambda = [](int left, int right) { return left * right; };
    expect(lambda(3, 4) == 12, "Function stores a lambda");

    Function<int(int, int)> pointer = add;
    expect(pointer(3, 4) == 7, "Function stores a function pointer");

    int base = 10;
    Function<int(int, int)> stateful = [base](int left, int right) { return base + left + right; };
    expect(stateful(1, 2) == 13, "Function stores a stateful lambda");

    Function<int(int, int)> copied =
        stateful;  // 拷贝须 clone 被擦除的具体对象，否则两者共享同一状态。
    expect(copied(2, 3) == 15, "Function copy clones the erased callable");
}

using CommandArg = Variant<int, double, std::string>;
using CommandHandler = Function<void(const std::string&, CommandArg)>;
using CommandEntry = Tuple<std::string, CommandHandler>;

// 依次尝试 int→double→string:consumed==size 确保整个 token 被吃掉，
// 否则 "12abc" 会被 stoi 误判为整数 12。空 token 返回空 Optional 表示无参数。
Optional<CommandArg> parse_argument(const std::string& token) {
    try {
        std::size_t consumed = 0;
        int int_value = std::stoi(token, &consumed);
        if (consumed == token.size()) {
            return CommandArg(int_value);
        }
    } catch (...) {
    }

    try {
        std::size_t consumed = 0;
        double double_value = std::stod(token, &consumed);
        if (consumed == token.size()) {
            return CommandArg(double_value);
        }
    } catch (...) {
    }

    if (!token.empty()) {
        return CommandArg(token);
    }
    return Optional<CommandArg>();
}

void run_command_parser_example() {
    std::cout << "== command parser integration ==\n";

    std::ostringstream log;
    CommandEntry echo("echo", CommandHandler([&log](const std::string& name, CommandArg arg) {
                          log << name << '=';
                          visit([&log](const auto& value) { log << value; }, arg);
                      }));

    CommandEntry square("square", CommandHandler([&log](const std::string& name, CommandArg arg) {
                            log << name << '=';
                            visit(
                                [&log](const auto& value) {
                                    using T = std::decay_t<decltype(value)>;
                                    // 只有算术类型能平方；if constexpr
                                    // 让非算术分支不被实例化，避免对 string 做 value*value。
                                    if constexpr (std::is_arithmetic_v<T>) {
                                        log << value * value;
                                    } else {
                                        log << "unsupported";
                                    }
                                },
                                arg);
                        }));

    std::map<std::string, CommandHandler> handlers;
    handlers.emplace(get<0>(echo), get<1>(echo));
    handlers.emplace(get<0>(square), get<1>(square));

    auto first_arg = parse_argument("7");
    expect(first_arg.has_value(), "parse_argument recognizes int arguments");
    handlers.at("square")("square", first_arg.value());
    expect(log.str() == "square=49", "command parser dispatches numeric command");

    log.str("");
    log.clear();  // str() 后还需 clear() 复位流状态位，否则残留的 eof/fail 位会吞掉后续写入。
    auto second_arg = parse_argument("hello");
    expect(second_arg.has_value(), "parse_argument recognizes string arguments");
    handlers.at("echo")("echo", second_arg.value());
    expect(log.str() == "echo=hello", "command parser dispatches string command");
}

}  // namespace demo

int main() {
    demo::run_optional_examples();
    std::cout << "----\n";

    demo::run_variant_examples();
    std::cout << "----\n";

    demo::run_function_examples();
    std::cout << "----\n";

    demo::run_command_parser_example();
    return 0;
}

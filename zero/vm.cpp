#include "vm.hpp"

#include "fmt/core.h"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"
#include "utils/file_utils.hpp"

#include <csignal>
#include <iostream>
#include <string>

using namespace zero;

void VM::run(std::string source) {
    // 词法解析
    auto lexer = Lexer(std::move(source));
    auto tokens = lexer.scan_tokens();

    // 语法解析
    Parser parser{tokens};
    auto program = parser.parse_program();

    if (parser.has_error()) {
        fmt::println("parse error");
        return;
    }

    // 解释器
    // TODO: 暂时是每一次执行都新创建一个解释器, 在REPL模式下不能利用上下文
    // Interpreter interpreter{};
    interpreter_->interpret(program);

    if (has_runtime_error_) {
        return;
    }
}

void VM::run_file(const std::string &file_path) {
    if (!utils::file_exists(file_path)) {
        fmt::println("File `{}` not exist", file_path);
        return;
    }

    auto source = utils::read_file(file_path);
    run(source);
}

void VM::run_REPL() {
    std::string user_input;

    // 注册信号处理函数
    signal(SIGINT, [](int signal) {
        if (signal == SIGINT) {
            fmt::println("Ctrl+C received. Exiting...");
            exit(signal);
        }
    });
    while (true) {
        fmt::print("> ");
        std::getline(std::cin, user_input);
        if (std::cin.eof()) {
            fmt::println("EOF reached. Exiting...");
            break;
        }
        if (std::cin.fail()) {
            fmt::println("Input error. Exiting...");
            break;
        }
        run(user_input);

        // 下一轮重置状态
        // has_parse_error = false;
    }
}

void VM::runtime_error(const RuntimeError &err) {
    fmt::println("[Line {}] {}", err.token.line, err.what());
    has_runtime_error_ = true;
}

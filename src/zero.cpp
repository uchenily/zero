#include "zero.hpp"

#include "cmdline.hpp"
#include "fmt/core.h"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"

#include <csignal>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace zero;
// 全局解释器
Interpreter VM::interpreter{};
bool VM::has_parse_error = false;
bool VM::has_runtime_error = false;

void VM::run(std::string source) {
    // 词法解析
    auto lexer = std::make_unique<Lexer>(std::move(source));
    auto tokens = lexer->scan_tokens();

    // 语法解析
    Parser parser{tokens};
    auto statements = parser.parse();

    if (has_parse_error) {
        return;
    }

    // 解释器
    // TODO: 暂时是每一次执行都新创建一个解释器, 在REPL模式下不能利用上下文
    // Interpreter interpreter{};
    interpreter.interpret(statements);

    if (has_runtime_error) {
        return;
    }
}

static bool file_exists(const std::string &file_path) {
    std::fstream file(file_path);
    return file.good();
}

static std::string read_file(const std::string &file_path) {
    std::fstream file(file_path, std::ios::in | std::ios::binary);
    std::stringstream buf;

    buf << file.rdbuf();

    return buf.str();
}

void VM::run_file(const std::string &file_path) {
    if (!file_exists(file_path)) {
        fmt::println("File `{}` not exist", file_path);
        return;
    }

    auto source = read_file(file_path);
    run(source);
}

static void signal_handler(int signal) {
    if (signal == SIGINT) {
        fmt::println("Ctrl+C received. Exiting...");
        exit(signal);
    }
}

void VM::run_REPL() {
    std::string user_input;

    // 注册信号处理函数
    signal(SIGINT, signal_handler);
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
        has_parse_error = false;
    }
}

void VM::report(unsigned int line,
                const std::string &pos,
                const std::string &reason) {
    fmt::println("[Line {}] Error {}: {}", line, pos, reason);
}

void VM::parse_error(const Token &token, const std::string &msg) {
    if (token.type == token_type::END) {
        report(token.line, "at end", msg);
    } else {
        report(token.line, "at `" + token.lexeme + "`", msg);
    }
    has_parse_error = true;
}

void VM::runtime_error(const RuntimeError &err) {
    fmt::println("[Line {}] {}", err.token.line, err.what());
    has_runtime_error = true;
}

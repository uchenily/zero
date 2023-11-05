#include "zero.hpp"

#include "cmdline.hpp"
#include "fmt/core.h"
#include "interpreter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "token.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace zero;
bool VM::has_parse_error = false;
bool VM::has_runtime_error = false;

void VM::run(std::string source) {
    // 词法解析
    auto lexer = std::make_unique<Lexer>(std::move(source));
    auto tokens = lexer->scan_tokens();

    // 语法解析
    Parser parser{tokens};
    auto expr = parser.parse();

    if (has_parse_error) {
        return;
    }

    // 解释器
    Interpreter interpreter;
    interpreter.interpret(expr);
}

void VM::run_file(const std::string &file) {
    std::fstream source(file, std::ios::in | std::ios::binary);
    std::stringstream buf;

    buf << source.rdbuf();
    run(buf.str());
}

void VM::run_REPL() {
    std::string user_input;
    while (true) {
        fmt::print("> ");
        std::getline(std::cin, user_input);
        run(user_input);
    }
}

void VM::report(unsigned int line,
                const std::string &pos,
                const std::string_view reason) {
    fmt::println("[Line {}] Error {}: {}", line, pos, reason);
}

void VM::parse_error(const Token &token, const std::string_view msg) {
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

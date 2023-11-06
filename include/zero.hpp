#pragma once
#include "interpreter.hpp"
#include "token.hpp"

#include <string>

using namespace zero;
class VM {

public:
    static void run_REPL();
    static void run_file(const std::string &file_path);
    // static void parse_error(unsigned int line, const std::string &msg);
    static void parse_error(const Token &token, const std::string &msg);
    static void runtime_error(const RuntimeError &err);

private:
    static void run(std::string source);
    static void report(unsigned int line,
                       const std::string &pos,
                       const std::string &reason);

private:
    static bool has_parse_error;
    static bool has_runtime_error;
};

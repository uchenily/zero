#pragma once
#include "interpreter.hpp"
#include "token.hpp"

#include <memory>
#include <string>

namespace zero {
struct RuntimeError;

class VM {
public:
    VM() { interpreter = std::make_unique<Interpreter>(); }

public:
    void run_REPL();
    void run_file(const std::string &file_path);
    // static void parse_error(unsigned int line, const std::string &msg);
    void parse_error(const Token &token, const std::string &msg);
    void runtime_error(const RuntimeError &err);

private:
    void run(std::string source);
    void report(unsigned int line,
                const std::string &pos,
                const std::string &reason);

private:
    std::unique_ptr<Interpreter> interpreter;
    bool has_runtime_error;
};
} // namespace zero

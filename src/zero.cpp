#include "cmdline.hpp"
#include "fmt/core.h"
#include "lexer.hpp"
#include "token.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace zero;

void usage() {
    fmt::println("./zero [file] [--help] [--verbose]");
    fmt::println("positions:");
    fmt::println("    file           parse and execute this file, optional");
    fmt::println("options:");
    fmt::println("    --help         print usage");
    fmt::println("    --verbose      verbose message");
}

void run(std::string source) {
    std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(std::move(source));
    std::vector<Token> tokens = lexer->scan_tokens();

    for (const auto &token : tokens) {
        fmt::println("{}", token.to_string());
    }
}

void run_file(const std::string &file) {
    std::fstream source(file, std::ios::in | std::ios::binary);
    std::stringstream buf;

    buf << source.rdbuf();
    run(buf.str());
}

void run_REPL() {
    std::string user_input;
    while (true) {
        fmt::print("> ");
        std::getline(std::cin, user_input);
        run(user_input);
    }
}

int main(int argc, char *argv[]) {
    bool verbose{};
    std::string file{};
    CmdLine::BoolOpt(&verbose, "verbose");
    CmdLine::StrPositional(&file);
    CmdLine::SetUsage(usage);
    int res = CmdLine::Parse(argc, argv);

    if (res != 0) {
        fmt::println("Command line parse error");
        return 1;
    }

    if (file.empty()) {
        run_REPL();
    } else {
        run_file(file);
    }
}

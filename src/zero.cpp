#include "cmdline.hpp"
#include "fmt/core.h"
#include "token.hpp"

#include <string>

using namespace zero;

void usage() {
    fmt::println("./zero [--help] [--verbose]");
    fmt::println("options:");
    fmt::println("    --help         print usage");
    fmt::println("    --verbose      verbose message");
}

int main(int argc, char *argv[]) {
    bool verbose{};
    CmdLine::BoolOpt(&verbose, "verbose");
    CmdLine::SetUsage(usage);
    int res = CmdLine::Parse(argc, argv);

    if (res == 0) {
        Token token_id{token_type::IDENTIFIER, "zero", "zero"};
        fmt::println("{}", token_id.to_string());

        Token token_true{token_type::TRUE, true, "true"};
        fmt::println("{}", token_true.to_string());

        Token token_num{token_type::NUMBER, 123, "123"};
        fmt::println("{}", token_num.to_string());
    }
}

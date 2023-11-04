#include "cmdline.hpp"
#include "fmt/core.h"

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
        // ...
    }
}

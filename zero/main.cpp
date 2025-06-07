#include "fmt/core.h"
#include "utils/cmdline.hpp"
#include "vm.hpp"

using namespace zero;

void usage() {
    fmt::println("./zero [file] [--help] [--verbose]");
    fmt::println("positions:");
    fmt::println("    file           parse and execute this file, optional");
    fmt::println("options:");
    fmt::println("    --help         print usage");
    fmt::println("    --verbose      verbose message");
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

    VM vm;
    if (file.empty()) {
        vm.run_REPL();
    } else {
        vm.run_file(file);
    }
}

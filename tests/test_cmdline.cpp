#include "fmt/core.h"
#include "zero/utils/cmdline.hpp"

using namespace std;
using namespace zero;

void usage() {
    fmt::println("./test_cmdline --ip 0.0.0.0 --port 8080 --verbose");
    fmt::println("options:");
    fmt::println("    --help         print usage");
    fmt::println("    --ip           listen ip");
    fmt::println("    --port         listen port");
    fmt::println("    --verbose      verbose message");
}

int main(int argc, char *argv[]) {
    string ip{};
    int port{};
    bool verbose{};
    CmdLine::StrOptRequired(&ip, "ip");
    CmdLine::IntOptRequired(&port, "port");
    CmdLine::BoolOpt(&verbose, "verbose");
    CmdLine::SetUsage(usage);
    int res = CmdLine::Parse(argc, argv);

    if (res == 0) {
        fmt::println("ip: {}", ip);
        fmt::println("port: {}", port);
        fmt::println("verbose: {}", verbose);
    }
}

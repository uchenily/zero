#include "token.hpp"

#include <string>

using namespace zero;
class VM {

public:
    static void run_REPL();
    static void run_file(const std::string &filepath);
    static void error(unsigned int line, const std::string &msg);
    static void error(const Token &token, std::string_view msg);

private:
    static void run(std::string source);
    static void
    report(unsigned int line, const std::string &pos, std::string_view reason);

private:
    static bool has_error;
};

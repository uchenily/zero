#include "zero/assert.hpp"
#include "zero/lexer.hpp"

#include <iostream>

using namespace zero;

int main() {
    std::string input = R"(
let five = 5;
let ten = 10;
fn add(x, y) {
    return x + y;
};

add(five, ten);
)";

    std::vector<std::string> expected = {
        // clang-format off
        "let",
        "five",
        "=",
        "5",
        ";",
        "let",
        "ten",
        "=",
        "10",
        ";",
        "fn",
        "add",
        "(",
        "x",
        ",",
        "y",
        ")",
        "{",
        "return",
        "x",
        "+",
        "y",
        ";",
        "}",
        ";",
        "add",
        "(",
        "five",
        ",",
        "ten",
        ")",
        ";",
        "",
        // clang-format on
    };
    auto lexer = Lexer(input);
    auto tokens = lexer.scan_tokens();
    int i = 0;
    for (auto &token : tokens) {
        expect(token.lexeme == expected[i++]);
        // std::cout << token.lexeme << " : " << i++ << '\n';
    }
}

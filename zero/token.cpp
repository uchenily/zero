#include "token.hpp"

#include "fmt/core.h"

namespace zero {
std::string Token::to_string() const {
    std::string literal_str;

    switch (type) {
        case (token_type::IDENTIFIER):
            literal_str = "id";
            break;
        case (token_type::STRING):
            // literal_str = std::any_cast<std::string>(literal);
            literal_str = "string";
            break;
        case (token_type::NUMBER):
            // literal_str = std::to_string(std::any_cast<int>(literal));
            literal_str = "number";
            break;
        case (token_type::TRUE):
            literal_str = "true";
            break;
        case (token_type::FALSE):
            literal_str = "false";
            break;
        default:
            literal_str = "nil";
    }

    return fmt::format("{} {}", literal_str, lexeme);
}
} // namespace zero

#include "token.hpp"

namespace zero {
std::string Token::to_string() const {
    std::string literal_str;

    switch (type) {
        case (token_type::IDENTIFIER):
            literal_str = lexeme;
            break;
        case (token_type::STRING):
            literal_str = std::any_cast<std::string>(literal);
            break;
        case (token_type::NUMBER):
            literal_str = std::to_string(std::any_cast<int>(literal));
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

    return std::to_string(static_cast<int>(type)) + " " + lexeme + " "
           + literal_str;
}
} // namespace zero

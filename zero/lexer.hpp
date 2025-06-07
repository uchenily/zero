#include "token.hpp"

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace zero {
class Lexer {
public:
    explicit Lexer(std::string source) : source(std::move(source)) {};
    std::vector<Token> scan_tokens();

private:
    bool is_at_end();
    void scan_token();
    char advance();
    void add_token(token_type type);
    void add_token(token_type type, const std::any &literal);
    bool match(char expected);
    char peek();
    void parse_string();
    void parse_number();
    static bool is_digit(char c);
    static bool is_alpha(char c);
    static bool is_alphanumeric(char c);
    char peek_next();
    void identifier();

private:
    std::vector<Token> tokens;
    unsigned int start = 0;
    unsigned int current = 0;
    unsigned int line = 1;
    std::string source;
    const std::map<std::string, token_type> keywords = {
        {"and", token_type::AND},
        {"or", token_type::OR},
        {"not", token_type::NOT},
        {"class", token_type::CLASS},
        {"else", token_type::ELSE},
        {"false", token_type::FALSE},
        {"for", token_type::FOR},
        {"fn", token_type::FN},
        {"if", token_type::IF},
        {"nil", token_type::NIL},
        {"or", token_type::OR},
        {"print", token_type::PRINT},
        {"return", token_type::RETURN},
        {"true", token_type::TRUE},
        {"let", token_type::LET},
        {"while", token_type::WHILE},
    };
};
} // namespace zero

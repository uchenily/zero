#include "lexer.hpp"

#include "fmt/core.h"

namespace zero {
std::vector<Token> Lexer::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        scan_token();
    }

    Token token(token_type::END, "", std::string(""), line);
    tokens.push_back(token);

    return tokens;
}

bool Lexer::is_at_end() { return current >= source.size(); }

void Lexer::scan_token() {
    char c = advance();

    switch (c) {
        case '(':
            add_token(token_type::LEFT_PAREN);
            break;
        case ')':
            add_token(token_type::RIGHT_PAREN);
            break;
        case '{':
            add_token(token_type::LEFT_BRACE);
            break;
        case '}':
            add_token(token_type::RIGHT_BRACE);
            break;
        case ',':
            add_token(token_type::COMMA);
            break;
        case '.':
            add_token(token_type::DOT);
            break;
        case '-':
            add_token(token_type::MINUS);
            break;
        case '+':
            add_token(token_type::PLUS);
            break;
        case ';':
            add_token(token_type::SEMICOLON);
            break;
        case '*':
            add_token(token_type::STAR);
            break;
        case '!':
            add_token(match('=') ? token_type::NOT_EQUAL : token_type::NOT);
            break;
        case '=':
            add_token(match('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL);
            break;
        case '<':
            add_token(match('=') ? token_type::LESS_EQUAL : token_type::LESS);
            break;
        case '>':
            add_token(match('=') ? token_type::GREATER_EQUAL
                                 : token_type::GREATER);
            break;
        case '/': {
            if (match('/')) {
                while (peek() != '\n' && !is_at_end()) {
                    advance();
                }
            } else {
                add_token(token_type::SLASH);
            }
        } break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '"':
            parse_string();
            break;
        default: {
            if (is_digit(c)) {
                parse_number();
            } else if (is_alpha(c)) {
                identifier();
            } else {
                fmt::println("Unexpected character in line {}.", line);
            }
        }
    }
}

char Lexer::advance() { return source.at(current++); }

void Lexer::add_token(token_type type) { add_token(type, std::string("")); }

void Lexer::add_token(token_type type, const std::any &literal) {
    auto text = source.substr(start, current - start);
    Token token(type, literal, text, line);
    tokens.push_back(token);
}

bool Lexer::match(char expected) {
    if (is_at_end()) {
        return false;
    }
    if (source.at(current) != expected) {
        return false;
    }
    current++;

    return true;
}

char Lexer::peek() {
    if (is_at_end()) {
        return '\0';
    }

    return source.at(current);
}

void Lexer::parse_string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }

    if (is_at_end()) {
        fmt::println("Unterminated string in line {}", line);
        return;
    }

    // Advance until closing quote
    advance();
    // Remove quotes from string
    auto value = source.substr(start + 1, current - start - 2);
    add_token(token_type::STRING, value);
}

void Lexer::parse_number() {
    while (static_cast<bool>(isdigit(peek()))) {
        advance();
    }

    // if (peek() == '.' && is_digit(peek_next())) {
    //     advance();
    //     while (is_digit(peek())) {
    //         advance();
    //     }
    // }

    auto number = std::stoi(source.substr(start, current - start));
    add_token(token_type::NUMBER, number);
}

bool Lexer::is_digit(const char c) { return c >= '0' && c <= '9'; }

char Lexer::peek_next() {
    if (current - 1 >= source.size()) {
        return '\0';
    }

    return source.at(current + 1);
}

void Lexer::identifier() {
    while (is_alphanumeric(peek())) {
        advance();
    }
    auto text = source.substr(start, current - start);
    auto found = keywords.find(text);
    token_type type{};

    if (found != keywords.end()) {
        type = found->second;
    } else {
        type = token_type::IDENTIFIER;
    }

    add_token(type);
}

bool Lexer::is_alpha(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::is_alphanumeric(const char c) { return is_alpha(c) || is_digit(c); }
} // namespace zero

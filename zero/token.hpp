#pragma once

#include <any>
#include <string>

namespace zero {

enum class token_type {
    // Single character tokens(i.e., dots, parenthesis, etc.
    LEFT_PAREN,  // (
    RIGHT_PAREN, // )
    LEFT_BRACE,  // {
    RIGHT_BRACE, // }
    COMMA,       // ,
    DOT,         // .
    MINUS,       // -
    PLUS,        // +
    COLON,       // :
    SEMICOLON,   // ;
    SLASH,       // /
    STAR,        // *
    // Single or two characters tokens(i.e., !=, ==, >=, etc.)
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    NOT,
    NOT_EQUAL,
    // Literals(i.e., strings, numbers, etc.)
    IDENTIFIER,
    STRING,
    NUMBER,
    // Builtin keywords(fn, if, else, etc.)
    AND,
    CLASS,
    ELSE,
    FALSE,
    FN,
    FOR,
    IF,
    NIL,
    OR,
    // PRINT,
    RETURN,
    TRUE,
    LET,
    WHILE,
    // End of file
    END
};

class Token {
public:
    Token(token_type type,
          std::any literal,
          std::string lexeme,
          unsigned int line)
        : type{type}, literal{std::move(literal)}, lexeme{std::move(lexeme)},
          line{line} {}

public:
    std::string to_string() const;

public:
    const token_type type;
    const std::any literal;   // 字面量, 字符串/数字/true/...
    const std::string lexeme; // 词位
    const unsigned int line;
};
} // namespace zero

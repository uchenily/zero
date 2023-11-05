#pragma once

#include "expr.hpp"
#include "token.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

namespace zero {
class Parser {
public:
    explicit Parser(const std::vector<Token> &tokens) : tokens(tokens){};
    std::unique_ptr<Expr> parse();

private:
    struct ParseError : public std::runtime_error {
        ParseError(const Token &token, std::string_view msg)
            : std::runtime_error{msg.data()}, token{token} {}

        const Token &token;
    };
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    template <class... T>
    bool match(T... type);
    Token consume(token_type type, const std::string_view msg);
    Token advance();
    Token peek();
    Token previous();
    bool is_at_end();
    bool check(token_type type);
    void synchronize();

    const std::vector<Token> &tokens;
    unsigned int current = 0;
};

} // namespace zero

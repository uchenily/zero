#pragma once

#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

namespace zero {
class Parser {
public:
    explicit Parser(const std::vector<Token> &tokens) : tokens(tokens){};
    std::vector<std::unique_ptr<Stmt>> parse();

private:
    struct ParseError : public std::runtime_error {
        ParseError(const Token &token, const std::string &msg)
            : std::runtime_error{msg}, token{token} {}

        const Token token;
    };
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> print_statement();
    std::unique_ptr<Stmt> var_declaration();
    std::unique_ptr<Stmt> expr_statement();
    std::unique_ptr<Stmt> if_statement();
    std::unique_ptr<Stmt> for_statement();
    std::unique_ptr<Stmt> while_statement();
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();
    template <class... T>
    bool match(T... type);
    Token consume(token_type type, const std::string &msg);
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

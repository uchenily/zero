#include "parser.hpp"

#include "zero.hpp"

#include <cassert>

namespace zero {

std::unique_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError &err) {
        return nullptr;
    }
}

std::unique_ptr<Expr> Parser::expression() { return equality(); }

std::unique_ptr<Expr> Parser::equality() {
    auto expr = comparison();

    while (match(token_type::NOT_EQUAL, token_type::EQUAL_EQUAL)) {
        Token op = previous();
        auto right = comparison();
        expr = std::make_unique<Binary>(
            std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison() {
    auto expr = term();

    while (match(token_type::GREATER,
                 token_type::GREATER_EQUAL,
                 token_type::LESS,
                 token_type::LESS_EQUAL)) {
        Token op = previous();
        auto right = term();
        expr = std::make_unique<Binary>(
            std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::term() {
    auto expr = factor();

    while (match(token_type::MINUS, token_type::PLUS)) {
        Token op = previous();
        auto right = factor();
        expr = std::make_unique<Binary>(
            std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::factor() {
    auto expr = unary();

    while (match(token_type::SLASH, token_type::STAR)) {
        Token op = previous();
        auto right = unary();
        expr = std::make_unique<Binary>(
            std::move(expr), std::move(op), std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary() {
    if (match(token_type::NOT, token_type::MINUS)) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<Unary>(std::move(op), std::move(right));
    }

    return primary();
}

std::unique_ptr<Expr> Parser::primary() {
    if (match(token_type::FALSE)) {
        return std::make_unique<Literal>(false);
    }
    if (match(token_type::TRUE)) {
        return std::make_unique<Literal>(true);
    }
    if (match(token_type::NIL)) {
        return std::make_unique<Literal>(nullptr);
    }
    if (match(token_type::NUMBER, token_type::STRING)) {
        return std::make_unique<Literal>(previous().literal);
    }
    if (match(token_type::LEFT_PAREN)) {
        std::unique_ptr<Expr> expr = expression();
        consume(token_type::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw ParseError(peek(), "Expect expression.");
}

template <class... T>
bool Parser::match(T... type) {
    assert((... && std::is_same_v<T, token_type>) );

    if ((... || check(type))) {
        advance();
        return true;
    }

    return false;
}

Token Parser::consume(token_type type, const std::string_view msg) {
    if (check(type)) {
        return advance();
    }

    throw ParseError(peek(), msg);
}

bool Parser::check(token_type type) {
    if (is_at_end()) {
        return false;
    }

    return peek().type == type;
}

Token Parser::advance() {
    if (!is_at_end()) {
        ++current;
    }

    return previous();
}

bool Parser::is_at_end() { return peek().type == token_type::END; }

Token Parser::peek() { return tokens.at(current); }

Token Parser::previous() { return tokens.at(current - 1); }

void Parser::synchronize() {
    advance();

    while (!is_at_end()) {
        if (previous().type == token_type::SEMICOLON) {
            return;
        }

        switch (peek().type) {
            case token_type::CLASS:
            case token_type::FN:
            case token_type::LET:
            case token_type::FOR:
            case token_type::IF:
            case token_type::WHILE:
            case token_type::PRINT:
            case token_type::RETURN:
                break;
        }
        advance();
    }
}
} // namespace zero

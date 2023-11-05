#pragma once

#include "token.hpp"

#include <any>
#include <memory>

namespace zero {
struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct Visitor {
    virtual std::any visit_binary_expr(Binary *expr) = 0;
    virtual std::any visit_grouping_expr(Grouping *expr) = 0;
    virtual std::any visit_literal_expr(Literal *expr) = 0;
    virtual std::any visit_unary_expr(Unary *expr) = 0;
    virtual ~Visitor() = default;
};

struct Expr {
    virtual std::any accept(Visitor &visitor) = 0;
};

struct Binary : Expr {
    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)){};

    std::any accept(Visitor &visitor) override {
        return visitor.visit_binary_expr(this);
    }

    const std::unique_ptr<Expr> left;
    const Token op;
    const std::unique_ptr<Expr> right;
};

struct Grouping : Expr {
    explicit Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)){};

    std::any accept(Visitor &visitor) override {
        return visitor.visit_grouping_expr(this);
    }

    const std::unique_ptr<Expr> expr;
};

struct Literal : Expr {
    explicit Literal(std::any value) : value(std::move(value)){};
    std::any accept(Visitor &visitor) override {
        return visitor.visit_literal_expr(this);
    }

    const std::any value;
};

struct Unary : Expr {
    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)){};
    std::any accept(Visitor &visitor) override {
        return visitor.visit_unary_expr(this);
    }

    const Token op;
    const std::unique_ptr<Expr> right;
};
} // namespace zero

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
    virtual std::any visitBinaryExpr(Binary *expr) = 0;
    virtual std::any visitGroupingExpr(Grouping *expr) = 0;
    virtual std::any visitLiteralExpr(Literal *expr) = 0;
    virtual std::any visitUnaryExpr(Unary *expr) = 0;
    virtual ~Visitor() = default;
};

struct Expr {
    virtual std::any accept(Visitor &visitor) = 0;
};

struct Binary : Expr {
    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)){};

    std::any accept(Visitor &visitor) override {
        return visitor.visitBinaryExpr(this);
    }

    const std::unique_ptr<Expr> left;
    const Token op;
    const std::unique_ptr<Expr> right;
};

struct Grouping : Expr {
    explicit Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)){};

    std::any accept(Visitor &visitor) override {
        return visitor.visitGroupingExpr(this);
    }

    const std::unique_ptr<Expr> expr;
};

struct Literal : Expr {
    explicit Literal(std::any value) : value(std::move(value)){};
    std::any accept(Visitor &visitor) override {
        return visitor.visitLiteralExpr(this);
    }

    const std::any value;
};

struct Unary : Expr {
    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)){};
    std::any accept(Visitor &visitor) override {
        return visitor.visitUnaryExpr(this);
    }

    const Token op;
    const std::unique_ptr<Expr> right;
};
} // namespace zero

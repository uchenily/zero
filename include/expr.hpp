#pragma once

#include "token.hpp"

#include <any>
#include <memory>
#include <vector>

namespace zero {
struct Binary;
struct Grouping;
struct Literal;
struct Logical;
struct Unary;
struct Variable;
struct Assign;
struct Call;

struct ExprVisitor {
    virtual std::any visit_binary_expr(Binary *expr) = 0;
    virtual std::any visit_grouping_expr(Grouping *expr) = 0;
    virtual std::any visit_literal_expr(Literal *expr) = 0;
    virtual std::any visit_logical_expr(Logical *expr) = 0;
    virtual std::any visit_unary_expr(Unary *expr) = 0;
    virtual std::any visit_variable_expr(Variable *expr) = 0;
    virtual std::any visit_assign_expr(Assign *expr) = 0;
    virtual std::any visit_call_expr(Call *expr) = 0;
    virtual ~ExprVisitor() = default;
};

struct Expr {
    virtual std::any accept(ExprVisitor &visitor) = 0;
};

struct Binary : Expr {
    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)){};

    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_binary_expr(this);
    }

    const std::unique_ptr<Expr> left;
    const Token op;
    const std::unique_ptr<Expr> right;
};

struct Grouping : Expr {
    explicit Grouping(std::unique_ptr<Expr> expr) : expr(std::move(expr)){};

    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_grouping_expr(this);
    }

    const std::unique_ptr<Expr> expr;
};

struct Literal : Expr {
    explicit Literal(std::any value) : value(std::move(value)){};
    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_literal_expr(this);
    }

    const std::any value;
};

struct Logical : Expr {
    Logical(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)){};

    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_logical_expr(this);
    }

    const std::unique_ptr<Expr> left;
    const Token op;
    const std::unique_ptr<Expr> right;
};

struct Unary : Expr {
    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)){};
    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_unary_expr(this);
    }

    const Token op;
    const std::unique_ptr<Expr> right;
};

struct Variable : Expr {
    explicit Variable(Token name) : name(std::move(name)){};

    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_variable_expr(this);
    }

    const Token name;
};

struct Assign : Expr {
    Assign(Token name, std::unique_ptr<Expr> value)
        : name(std::move(name)), value(std::move(value)){};

    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_assign_expr(this);
    }

    const Token name;
    const std::unique_ptr<Expr> value;
};

struct Call : Expr {
    Call(std::unique_ptr<Expr> callee,
         std::vector<std::unique_ptr<Expr>> arguments)
        : callee{std::move(callee)}, arguments{std::move(arguments)} {};

    std::any accept(ExprVisitor &visitor) override {
        return visitor.visit_call_expr(this);
    }

    const std::unique_ptr<Expr> callee;
    const std::vector<std::unique_ptr<Expr>> arguments;
};

} // namespace zero

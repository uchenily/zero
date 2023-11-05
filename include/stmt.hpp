#pragma once

#include "expr.hpp"

#include <any>
#include <memory>
#include <vector>

struct Block;
struct Expression;
struct Print;
struct Var;

using namespace zero;
class StmtVisitor {
public:
    virtual std::any visit_block_stmt(Block *stmt) = 0;
    virtual std::any visit_expression_stmt(Expression *stmt) = 0;
    virtual std::any visit_print_stmt(Print *stmt) = 0;
    virtual std::any visit_var_stmt(Var *stmt) = 0;
    virtual ~StmtVisitor() = default;
};

class Stmt {
public:
    virtual std::any accept(StmtVisitor &visitor) = 0;
};

struct Block : Stmt {
    explicit Block(std::vector<std::unique_ptr<Stmt>> statements)
        : statements(std::move(statements)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_block_stmt(this);
    }

    const std::vector<std::unique_ptr<Stmt>> statements;
};

struct Expression : Stmt {
    explicit Expression(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_expression_stmt(this);
    }

    const std::unique_ptr<Expr> expression;
};

struct Print : Stmt {
    explicit Print(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_print_stmt(this);
    }

    const std::unique_ptr<Expr> expression;
};

struct Var : Stmt {
    Var(Token name, std::unique_ptr<Expr> initializer)
        : name(std::move(name)), initializer(std::move(initializer)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_var_stmt(this);
    }

    const Token name;
    const std::unique_ptr<Expr> initializer;
};

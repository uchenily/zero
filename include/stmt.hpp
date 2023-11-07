#pragma once

#include "expr.hpp"

#include <any>
#include <memory>
#include <vector>

namespace zero {
struct Block;
struct Expression;
struct Print;
struct Var;
struct If;
struct While;
struct Function;
struct Return;

class StmtVisitor {
public:
    virtual std::any visit_block_stmt(Block *stmt) = 0;
    virtual std::any visit_expression_stmt(Expression *stmt) = 0;
    virtual std::any visit_print_stmt(Print *stmt) = 0;
    virtual std::any visit_var_stmt(Var *stmt) = 0;
    virtual std::any visit_if_stmt(If *stmt) = 0;
    virtual std::any visit_while_stmt(While *stmt) = 0;
    virtual std::any visit_function_stmt(Function *stmt) = 0;
    virtual std::any visit_return_stmt(Return *stmt) = 0;
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

struct If : Stmt {
    If(std::unique_ptr<Expr> condition,
       std::unique_ptr<Stmt> then_branch,
       std::unique_ptr<Stmt> else_branch)
        : condition(std::move(condition)), then_branch(std::move(then_branch)),
          else_branch(std::move(else_branch)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_if_stmt(this);
    }

    const std::unique_ptr<Expr> condition;
    const std::unique_ptr<Stmt> then_branch;
    const std::unique_ptr<Stmt> else_branch;
};

struct While : Stmt {
    While(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body)
        : condition(std::move(condition)), body(std::move(body)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_while_stmt(this);
    }

    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};

struct Function : Stmt {
    Function(Token name,
             std::vector<Token> params,
             std::vector<std::unique_ptr<Stmt>> body)
        : name(std::move(name)), params(std::move(params)),
          body(std::move(body)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_function_stmt(this);
    }

    const Token name;
    const std::vector<Token> params;
    const std::vector<std::unique_ptr<Stmt>> body;
};

struct Return : Stmt {
    Return(Token keyword, std::unique_ptr<Expr> value)
        : keyword(std::move(keyword)), value(std::move(value)){};

    std::any accept(StmtVisitor &visitor) override {
        return visitor.visit_return_stmt(this);
    }

    const Token keyword;
    const std::unique_ptr<Expr> value;
};

} // namespace zero

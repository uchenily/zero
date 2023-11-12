#pragma once
#include "environment.hpp"
#include "expr.hpp"
#include "function.hpp"
#include "stmt.hpp"

namespace zero {
struct RuntimeError : public std::runtime_error {
    RuntimeError(const Token &token, const std::string &msg)
        : std::runtime_error{msg}, token{token} {}

    const Token token;
};

class Interpreter : public ExprVisitor, public StmtVisitor {
    friend ZeroFunction;

public:
    Interpreter() : environment{std::make_unique<Environment>()} {}
    void interpret(const std::vector<std::unique_ptr<Stmt>> &stmts);
    // Expr抽象类方法
    std::any visit_binary_expr(Binary *expr) override;
    std::any visit_grouping_expr(Grouping *expr) override;
    std::any visit_literal_expr(Literal *expr) override;
    std::any visit_logical_expr(Logical *expr) override;
    std::any visit_unary_expr(Unary *expr) override;
    std::any visit_variable_expr(Variable *expr) override;
    std::any visit_assign_expr(Assign *expr) override;
    std::any visit_call_expr(Call *expr) override;

    // Stmt抽象类方法
    std::any visit_block_stmt(Block *stmt) override;
    std::any visit_expression_stmt(Expression *stmt) override;
    std::any visit_print_stmt(Print *stmt) override;
    std::any visit_var_stmt(Var *stmt) override;
    std::any visit_if_stmt(If *stmt) override;
    std::any visit_while_stmt(While *stmt) override;
    std::any visit_function_stmt(Function *stmt) override;
    std::any visit_return_stmt(Return *stmt) override;

private:
    // 表达式求值
    std::any evaluate(const std::unique_ptr<Expr> &expr);
    // 执行语句
    void execute(const std::unique_ptr<Stmt> &stmt);
    void execute_block(const std::vector<std::unique_ptr<Stmt>> &stmts,
                       std::unique_ptr<Environment> env);

    static void check_number_operand(const Token &op, const std::any &operand);
    static void check_number_operands(const Token &op,
                                      const std::any &left,
                                      const std::any &right);
    static bool is_truthy(const std::any &object);
    static bool is_equal(const std::any &a, const std::any &b);
    static std::string stringify(const std::any &object);

private:
    std::unique_ptr<Environment> environment; // current environment
};
} // namespace zero

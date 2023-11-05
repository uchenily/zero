#pragma once
#include "expr.hpp"

namespace zero {
struct RuntimeError : public std::runtime_error {
    RuntimeError(const Token &token, std::string_view msg)
        : std::runtime_error{msg.data()}, token{token} {}

    const Token &token;
};

class Interpreter : public ExprVisitor {
public:
    void interpret(const std::unique_ptr<Expr> &expr);
    std::any visit_binary_expr(Binary *expr) override;
    std::any visit_grouping_expr(Grouping *expr) override;
    std::any visit_literal_expr(Literal *expr) override;
    std::any visit_unary_expr(Unary *expr) override;
    std::any visit_variable_expr(Variable *expr) override;

private:
    std::any evaluate(const std::unique_ptr<Expr> &expr);
    static void check_number_operand(const Token &op, const std::any &operand);
    static void check_number_operands(const Token &op,
                                      const std::any &left,
                                      const std::any &right);
    static bool is_truthy(const std::any &object);
    static bool is_equal(const std::any &a, const std::any &b);
    static std::string stringify(const std::any &object);
};
} // namespace zero

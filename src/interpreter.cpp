#include "interpreter.hpp"

#include "assert.hpp"
#include "fmt/core.h"
#include "function.hpp"
#include "token.hpp"
#include "zero.hpp"

namespace zero {
void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>> &stmts) {
    try {
        for (const auto &stmt : stmts) {
            execute(stmt);
        }
    } catch (const RuntimeError &err) {
        VM::runtime_error(err);
    }
}

std::any Interpreter::evaluate(const std::unique_ptr<Expr> &expr) {
    return expr->accept(*this);
}

void Interpreter::execute(const std::unique_ptr<Stmt> &stmt) {
    stmt->accept(*this);
}

void Interpreter::execute_block(const std::vector<std::unique_ptr<Stmt>> &stmts,
                                std::unique_ptr<Environment> env) {
    // expect(this->environment != nullptr);
    // expect(env != nullptr);
    // 保存Env
    // std::unique_ptr<Environment> previous = std::move(this->environment);
    // try {
    //    this->environment = std::move(env);

    //    for (const auto &stmt : stmts) {
    //        execute(stmt);
    //    }
    //} catch (...) {
    //    // this->environment = std::move(previous);
    //    this->environment = this->environment->get_enclosing();
    //    throw;
    //}
    EnviromentGuard guard{std::move(this->environment), std::move(env)};
    for (const auto &stmt : stmts) {
        execute(stmt);
    }

    // 恢复Env
    // env = std::move(this->environment);
    // this->environment = std::move(previous);
    // expect(this->environment != nullptr);
    // expect(env != nullptr);
    // 出block前, 将之前的env恢复 (按照出栈来理解)
    // this->environment = this->environment->get_enclosing();
}

std::any Interpreter::visit_binary_expr(Binary *expr) {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
        case token_type::NOT_EQUAL:
            return !is_equal(left, right);
        case token_type::EQUAL_EQUAL:
            return is_equal(left, right);
        case token_type::GREATER:
            check_number_operands(expr->op, left, right);
            return std::any_cast<int>(left) > std::any_cast<int>(right);
        case token_type::GREATER_EQUAL:
            check_number_operands(expr->op, left, right);
            return std::any_cast<int>(left) >= std::any_cast<int>(right);
        case token_type::LESS:
            check_number_operands(expr->op, left, right);
            return std::any_cast<int>(left) < std::any_cast<int>(right);
        case token_type::LESS_EQUAL:
            check_number_operands(expr->op, left, right);
            return std::any_cast<int>(left) <= std::any_cast<int>(right);
        case token_type::MINUS:
            check_number_operands(expr->op, left, right);
            return std::any_cast<int>(left) - std::any_cast<int>(right);
        case token_type::PLUS:
            if (left.type() == typeid(int) && right.type() == typeid(int)) {
                return std::any_cast<int>(left) + std::any_cast<int>(right);
            }
            if (left.type() == typeid(std::string)
                && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left)
                       + std::any_cast<std::string>(right);
            }

            throw RuntimeError(expr->op,
                               "Operands must be two numbers or two strings.");
        case token_type::SLASH:
            check_number_operands(expr->op, left, right);
            return std::any_cast<int>(left) / std::any_cast<int>(right);
        case token_type::STAR:
            check_number_operands(expr->op, left, right);
            return std::any_cast<int>(left) * std::any_cast<int>(right);
        default:
            break;
    }

    return {};
}

std::any Interpreter::visit_grouping_expr(Grouping *expr) {
    return evaluate(expr->expr);
}

std::any Interpreter::visit_literal_expr(Literal *expr) { return expr->value; }

std::any Interpreter::visit_logical_expr(Logical *expr) {
    auto left = evaluate(expr->left);

    if (expr->op.type == token_type::OR) {
        if (is_truthy(left)) {
            return left;
        }
    } else {
        if (!is_truthy(left)) {
            return left;
        }
    }

    return evaluate(expr->right);
}

std::any Interpreter::visit_unary_expr(Unary *expr) {
    std::any right = evaluate(expr->right);

    switch (expr->op.type) {
        case token_type::NOT:
            return !is_truthy(right);
        case token_type::MINUS:
            check_number_operand(expr->op, right);
            return -std::any_cast<int>(right);
        default:
            break;
    }

    return {};
}

std::any Interpreter::visit_variable_expr(Variable *expr) {
    return environment->get(expr->name);
}

std::any Interpreter::visit_assign_expr(Assign *expr) {
    std::any value = evaluate(expr->value);
    environment->assign(expr->name, value);

    return value;
}

std::any Interpreter::visit_call_expr(Call *expr) {
    std::any callee = evaluate(expr->callee);
    std::vector<std::any> arguments;
    for (const auto &argument : expr->arguments) {
        arguments.push_back(evaluate(argument));
    }

    if (callee.type() == typeid(ZeroFunction)) {
        auto function = std::any_cast<ZeroFunction>(callee);
        return function.call(*this, std::move(arguments));
    }
    // TODO
    throw RuntimeError(Token{token_type::FN, {}, "fn", 0},
                       "Can only call functions and classes.");
}

std::any Interpreter::visit_block_stmt(Block *stmt) {
    // 进入block, 创建一个新的environment
    // execute_block(stmt->statements, std::make_unique<Environment>());
    // 进入block前, 创建一个新的env, 需要包含当前env (按照入栈理解)
    execute_block(stmt->statements,
                  std::make_unique<Environment>(environment.get()));

    return {};
}

std::any Interpreter::visit_expression_stmt(Expression *stmt) {
    evaluate(stmt->expression);

    return {};
}

std::any Interpreter::visit_print_stmt(Print *stmt) {
    std::any value = evaluate(stmt->expression);
    fmt::println("{}", stringify(value));

    return {};
}

std::any Interpreter::visit_var_stmt(Var *stmt) {
    std::any value = nullptr;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }

    environment->define(stmt->name.lexeme, std::move(value));

    return {};
}

std::any Interpreter::visit_if_stmt(If *stmt) {
    if (is_truthy(evaluate(stmt->condition))) {
        execute(stmt->then_branch);
    } else if (stmt->else_branch != nullptr) {
        execute(stmt->else_branch);
    }

    return {};
}

std::any Interpreter::visit_while_stmt(While *stmt) {
    while (is_truthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }

    return {};
}

std::any Interpreter::visit_function_stmt(Function *stmt) {
    auto function = ZeroFunction(stmt, environment.get());
    // NOTE: 转std::any需要这个类似有拷贝构造函数
    environment->define(stmt->name.lexeme, function);

    return {};
}

std::any Interpreter::visit_return_stmt(Return *stmt) {
    std::any value = nullptr;
    if (stmt->value != nullptr) {
        value = evaluate(stmt->value);
    }

    // 使用异常-捕获的方式通知函数返回
    // FIXME: 更好的方式?
    throw ZeroReturn{value};
}

void Interpreter::check_number_operand(const Token &op,
                                       const std::any &operand) {
    if (operand.type() == typeid(int)) {
        return;
    }
    throw RuntimeError(op, "Operand must be a number.");
}

void Interpreter::check_number_operands(const Token &op,
                                        const std::any &left,
                                        const std::any &right) {
    if (left.type() == typeid(int) && right.type() == typeid(int)) {
        return;
    }

    throw RuntimeError(op, "Operands must be numbers.");
}

bool Interpreter::is_truthy(const std::any &object) {
    if (object.type() == typeid(nullptr)) {
        return false;
    }
    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object);
    }

    return true;
}

bool Interpreter::is_equal(const std::any &a, const std::any &b) {
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) {
        return true;
    }
    if (a.type() == typeid(nullptr)) {
        return false;
    }
    if (a.type() == typeid(std::string) && b.type() == typeid(std::string)) {
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }
    if (a.type() == typeid(int) && b.type() == typeid(int)) {
        return std::any_cast<int>(a) == std::any_cast<int>(b);
    }
    if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }

    return false;
}

std::string Interpreter::stringify(const std::any &object) {
    if (object.type() == typeid(nullptr)) {
        return "nil";
    }
    if (object.type() == typeid(int)) {
        std::string text = std::to_string(std::any_cast<int>(object));
        if (text[text.length() - 2] == '.' && text[text.length() - 1] == '0') {
            text = text.substr(0, text.length() - 2);
        }

        return text;
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }
    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object) ? "true" : "false";
    }
    if (object.type() == typeid(ZeroFunction)) {
        return std::any_cast<ZeroFunction>(object).to_string();
    }

    return "Error in 'stringify': object type not supported.";
}
} // namespace zero

#pragma once
#include "ast/expr.hpp"
#include "ast/stmt.hpp"
#include "environment.hpp"
#include "function.hpp"
#include "parser.hpp"
#include "vm.hpp"

#include <system_error>

namespace zero {
class VM;

struct RuntimeError : public std::runtime_error {
    RuntimeError(const Token &token, const std::string &msg)
        : std::runtime_error{msg}, token{token} {}

    const Token token;
};

class Interpreter : public ExprVisitor, public StmtVisitor {
    friend ZeroFunction;

public:
    Interpreter() {
        globals_ = std::make_unique<Environment>();
        environment_
            = globals_.get(); // 初始化的时候, environment也就是globals环境
        // 在函数调用进入时, environment会发生改变
        // 在函数调用完成时, environment又恢复回来(globals环境)
        register_functions();
    }

public:
    void interpret(const std::unique_ptr<Program> &program);
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
    // std::any visit_print_stmt(Print *stmt) override;
    std::any visit_var_stmt(Var *stmt) override;
    std::any visit_if_stmt(If *stmt) override;
    std::any visit_while_stmt(While *stmt) override;
    std::any visit_function_stmt(Function *stmt) override;
    std::any visit_return_stmt(Return *stmt) override;

private:
    // 表达式求值
    std::any evaluate(Expr &expr);
    // 执行语句
    void execute(Stmt &stmt);
    void execute_block(const std::vector<std::unique_ptr<Stmt>> &stmts,
                       Environment *env);

    static void check_number_operand(const Token &op, const std::any &operand);
    static void check_number_operands(const Token &op,
                                      const std::any &left,
                                      const std::any &right);
    static bool is_truthy(const std::any &object);
    static bool is_equal(const std::any &a, const std::any &b);
    static std::string stringify(const std::any &object);

    // helper function
    auto get_globals() { return globals_.get(); };
    void register_functions();

private:
    class EnviromentGuard {
    public:
        // This simulates "finally" keyword usage in executeBlock of Java
        // version "execute" can throw "ReturnException" and we need to unwind
        // the stack and return to previous enviroment on each scope exit
        EnviromentGuard(Interpreter *interpreter, Environment *new_env)
            : interpreter{interpreter}, previous{interpreter->environment_} {
            // 设置新环境
            interpreter->environment_ = new_env;
        }

        // 恢复原环境
        ~EnviromentGuard() { interpreter->environment_ = previous; }

    private:
        Interpreter *interpreter;
        Environment *previous;
    };

private:
    VM *vm_;
    Environment *environment_; // 解释器当前环境
    std::unique_ptr<Environment>
        globals_; // 解释器global环境, 初始化后指针不再改变
};
} // namespace zero

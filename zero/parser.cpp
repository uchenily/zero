#include "parser.hpp"

#include "token.hpp"

#include <cassert>

#include <fmt/base.h>

namespace zero {

std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    // program -> declaration* END
    std::vector<std::unique_ptr<Stmt>> statements;
    try {
        while (!is_at_end()) {
            statements.push_back(declaration());
        }
    } catch (const ParseError &err) {
        parse_error(err.token, err.what());
        // synchronize();
    }
    return statements;
}

void Parser::parse_error(const Token &token, const std::string &msg) {
    auto report = [](unsigned int line,
                     const std::string &pos,
                     const std::string &reason) {
        fmt::println("[Line {}] Error {}: {}", line, pos, reason);
    };

    if (token.type == token_type::END) {
        report(token.line, "at end", msg);
    } else {
        report(token.line, "at `" + token.lexeme + "`", msg);
    }
    has_parse_error_ = true;
}

std::unique_ptr<Expr> Parser::expression() {
    // expression -> assignment
    return assignment();
}

std::unique_ptr<Stmt> Parser::declaration() {
    // declaration -> var_declaration | func_declaration | statement
    // try {
    if (match(token_type::LET)) {
        return var_declaration();
    }
    if (match(token_type::FN)) {
        return func_declaration();
    }

    return statement();
    //} catch (const ParseError &err) {
    // vm_->parse_error(err.token, err.what());
    // synchronize();
    // return nullptr;
    // }
}

std::unique_ptr<Stmt> Parser::statement() {
    // statement -> if_stmt | for_stmt | while_stmt | print_stmt | block |
    // return_stmt | expr_stmt
    if (match(token_type::IF)) {
        return if_statement();
    }
    if (match(token_type::FOR)) {
        return for_statement();
    }
    if (match(token_type::WHILE)) {
        return while_statement();
    }
    // if (match(token_type::PRINT)) {
    //     return print_statement();
    // }
    if (match(token_type::LEFT_BRACE)) {
        return std::make_unique<Block>(block());
    }
    if (match(token_type::RETURN)) {
        return return_statement();
    }

    return expr_statement();
}

std::unique_ptr<Stmt> Parser::for_statement() {
    // for_stmt -> "for" "(" ( var_decl | expr_stmt | ";" )
    //             expression? ";"
    //             expression? ")" stmt
    consume(token_type::LEFT_PAREN, "Expect '(' after for");

    std::unique_ptr<Stmt> initializer;
    if (match(token_type::SEMICOLON)) {
        initializer = nullptr;
    } else if (match(token_type::LET)) {
        initializer = var_declaration();
    } else {
        initializer = expr_statement();
    }

    std::unique_ptr<Expr> cond = nullptr;
    if (!check(token_type::SEMICOLON)) {
        cond = expression();
    }
    consume(token_type::SEMICOLON, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment = nullptr;
    if (!check(token_type::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(token_type::RIGHT_PAREN, "Expect ')' after for clauses.");
    std::unique_ptr<Stmt> body = statement();

    if (increment != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(body));
        statements.push_back(
            std::make_unique<Expression>(std::move(increment)));
        body = std::make_unique<Block>(std::move(statements));
    }

    if (cond == nullptr) {
        cond = std::make_unique<Literal>(true);
    }
    body = std::make_unique<While>(std::move(cond), std::move(body));

    if (initializer != nullptr) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<Block>(std::move(statements));
    }

    return body;
}

std::unique_ptr<Stmt> Parser::if_statement() {
    // if_stmt -> "if" "(" expression ")" statement ( "else" statement )?
    consume(token_type::LEFT_PAREN, "Expect '(' after if.");
    std::unique_ptr<Expr> cond = expression();
    consume(token_type::RIGHT_PAREN, "Expect ')' after if condition.");

    std::unique_ptr<Stmt> then_branch = statement();
    std::unique_ptr<Stmt> else_branch = nullptr;
    if (match(token_type::ELSE)) {
        else_branch = statement();
    }

    return std::make_unique<If>(
        std::move(cond), std::move(then_branch), std::move(else_branch));
}

std::unique_ptr<Stmt> Parser::while_statement() {
    // while_stmt -> "while" "(" expression ")" statement
    consume(token_type::LEFT_PAREN, "Expect '(' after while.");
    std::unique_ptr<Expr> cond = expression();
    consume(token_type::RIGHT_PAREN, "Expect ')' after condition.");
    std::unique_ptr<Stmt> body = statement();

    return std::make_unique<While>(std::move(cond), std::move(body));
}

// std::unique_ptr<Stmt> Parser::print_statement() {
//     // print_stmt -> "print" expression ";"
//     auto value = expression();
//     consume(token_type::SEMICOLON, "Expect ';' after value.");
//
//     return std::make_unique<Print>(std::move(value));
// }

std::unique_ptr<Stmt> Parser::var_declaration() {
    // var_declaration -> "let" IDENTIFIER ("=" expression)? ";"
    Token name = consume(token_type::IDENTIFIER, "Expect variable name.");
    std::unique_ptr<Expr> initializer = nullptr;

    if (match(token_type::EQUAL)) {
        initializer = expression();
    }

    consume(token_type::SEMICOLON, "Expect ';' after variable declaration.");

    return std::make_unique<Var>(std::move(name), std::move(initializer));
}

std::unique_ptr<Stmt> Parser::expr_statement() {
    // expr_stmt -> expression ";"
    auto expr = expression();
    consume(token_type::SEMICOLON, "Expect ';' after expression.");

    return std::make_unique<Expression>(std::move(expr));
}

std::vector<std::unique_ptr<Stmt>> Parser::block() {
    // block -> "{" declaration* "}"
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(token_type::RIGHT_BRACE) && !is_at_end()) {
        statements.push_back(declaration());
    }

    consume(token_type::RIGHT_BRACE, "Expect '}' after block.");

    return statements;
}

std::unique_ptr<Stmt> Parser::return_statement() {
    // return_stmt -> "return" expression? ";"
    Token keyword = previous();
    std::unique_ptr<Expr> value = nullptr;
    if (!check(token_type::SEMICOLON)) {
        value = expression();
    }

    consume(token_type::SEMICOLON, "Expect `;` after return value.");
    return std::make_unique<Return>(std::move(keyword), std::move(value));
}

std::unique_ptr<Expr> Parser::assignment() {
    // assignment -> IDENTIFIER "=" assignment | logic_or
    auto expr = or_expression();

    if (match(token_type::EQUAL)) {
        Token equals = previous();
        auto value = assignment();

        if (auto *e = dynamic_cast<Variable *>(expr.get())) {
            Token name = e->name;
            return std::make_unique<Assign>(std::move(name), std::move(value));
        }

        parse_error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::or_expression() {
    // logic_or -> logic_and ( "or" logic_and )*
    auto expr = and_expression();

    while (match(token_type::OR)) {
        auto op = previous();
        auto right = and_expression();
        expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::and_expression() {
    // logic_and -> equality ( "and" equality )*
    auto expr = equality();

    while (match(token_type::AND)) {
        auto op = previous();
        auto right = equality();
        expr = std::make_unique<Logical>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality() {
    // equality -> comparison ( ( "!=" | "==" ) comparison )*
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
    // comparison -> term ( ( ">" | ">=" | "<" | "<=" ) term )*
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
    // term -> factor ( ( "-" | "+" ) factor )*
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
    // factor -> unary ( ( "/" | "*" ) unary )*
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
    // unary -> ( "!" | "-" ) unary | call
    if (match(token_type::NOT, token_type::MINUS)) {
        Token op = previous();
        auto right = unary();
        return std::make_unique<Unary>(std::move(op), std::move(right));
    }

    return call();
}

std::unique_ptr<Expr> Parser::call() {
    // call -> primary ( "(" arguments? ")" )*
    std::unique_ptr<Expr> expr = primary();
    while (true) {
        if (match(token_type::LEFT_PAREN)) {
            expr = finish_call(std::move(expr));
        } else {
            break;
        }
    }
    return expr;
}

std::unique_ptr<Expr> Parser::finish_call(std::unique_ptr<Expr> callee) {
    std::vector<std::unique_ptr<Expr>> arguments;
    if (!check(token_type::RIGHT_PAREN)) {
        while (true) {
            arguments.push_back(expression());
            if (!match(token_type::COMMA)) {
                break;
            }
        }
    }

    consume(token_type::RIGHT_PAREN, "Expect `)` after arguments.");

    return std::make_unique<Call>(std::move(callee), std::move(arguments));
}

std::unique_ptr<Expr> Parser::primary() {
    // primary -> NUMBER | STRING | "false" | "true" | "nil" | IDENTIFIER | "("
    // expression ")"
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
    if (match(token_type::IDENTIFIER)) {
        return std::make_unique<Variable>(previous());
    }
    if (match(token_type::LEFT_PAREN)) {
        std::unique_ptr<Expr> expr = expression();
        consume(token_type::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw ParseError(peek(), "Expect expression.");
}

std::unique_ptr<Function> Parser::func_declaration() {
    // function -> "fn" IDENTIFIER "(" parameters? ")" block
    Token name = consume(token_type::IDENTIFIER, "Expect function name.");
    consume(token_type::LEFT_PAREN, "Expection `(` after function name.");
    std::vector<Token> parameters;
    if (!check(token_type::RIGHT_PAREN)) {
        while (true) {
            parameters.push_back(
                consume(token_type::IDENTIFIER, "Expect parameter name."));
            if (!match(token_type::COMMA)) {
                break;
            }
        }
    }
    consume(token_type::RIGHT_PAREN, "Expect `)` after parameters.");
    consume(token_type::LEFT_BRACE, "Expect `{` before function body.");
    std::vector<std::unique_ptr<Stmt>> body = block();

    return std::make_unique<Function>(
        std::move(name), std::move(parameters), std::move(body));
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

Token Parser::consume(token_type type, const std::string &msg) {
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
            // case token_type::PRINT:
            case token_type::RETURN:
                return;
            default:
                break;
        }
        advance();
    }
}
} // namespace zero

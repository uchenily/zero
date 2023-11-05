#pragma once

#include "token.hpp"

#include <any>
#include <map>
#include <memory>

namespace zero {
class Environment {

public:
    Environment() : enclosing(nullptr){};
    explicit Environment(std::unique_ptr<Environment> enclosing)
        : enclosing(std::move(enclosing)){};
    std::any get(const Token &name);
    void assign(const Token &name, std::any value);
    void define(const std::string &name, std::any value);

    std::unique_ptr<Environment> get_enclosing() {
        return std::move(this->enclosing);
    }

private:
    std::unique_ptr<Environment> enclosing;
    std::map<std::string, std::any> values;
};
} // namespace zero

#pragma once

#include "environment.hpp"

#include <any>
#include <string>
#include <vector>

namespace zero {

// 前置声明
class Interpreter;
class Function; // Statement

// 接口
class Callable {
public:
    virtual std::any call(Interpreter &interpreter,
                          std::vector<std::any> arguments)
        = 0;
    virtual std::string to_string() = 0;
};

class ZeroFunction : public Callable {
public:
    ZeroFunction() = default;
    ZeroFunction(Function *declaration, Environment *closure)
        : declaration{declaration}, closure{closure} {};

public:
    std::string to_string() override;
    std::any call(Interpreter &interpreter,
                  std::vector<std::any> arguments) override;

private:
    Function *declaration;
    Environment *closure;
};

class ZeroReturn {
public:
    const std::any value;
};
} // namespace zero

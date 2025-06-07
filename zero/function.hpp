#pragma once

#include "environment.hpp"

#include <any>
#include <functional>
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

    // 基类析构函数定义成虚函数
    virtual ~Callable() = default;
};

// 普通函数
class ZeroFunction : public Callable {
public:
    explicit ZeroFunction(Function *declaration) : declaration{declaration} {};

public:
    std::string to_string() override;
    std::any call(Interpreter &interpreter,
                  std::vector<std::any> arguments) override;

private:
    Function *declaration;
    // Environment *closure;
};

// 原生函数
using NativeFuncType = std::function<std::any(const std::vector<std::any> &)>;
class NativeFunction : public Callable {
public:
    explicit NativeFunction(NativeFuncType native_func)
        : native_func{std::move(native_func)} {}
    std::string to_string() override;
    std::any call(Interpreter &interpreter,
                  std::vector<std::any> arguments) override;

private:
    NativeFuncType native_func;
};

class ZeroReturn {
public:
    const std::any value;
};
} // namespace zero

#pragma once

#include "token.hpp"

#include <any>
#include <map>
#include <memory>

namespace zero {
class Environment {

public:
    Environment() = default;
    explicit Environment(Environment *enclosing) : enclosing{enclosing} {};

public:
    // 获取一个环境变量
    std::any get(const Token &name);
    // 给环境变量赋值
    void assign(const Token &name, std::any value);
    // 在当前环境定义一个变量/函数
    void define(const std::string &name, std::any value);

private:
    Environment *enclosing{};               // 外层的封闭环境
    std::map<std::string, std::any> values; // 根据token的词位信息存储变量值
    // 例如, fn add(a, b) {...}; add(1, 2);
    // values中会存放键值对 "a": 1, "b": 2
};

} // namespace zero

#include "function.hpp"

#include "interpreter.hpp"
#include "stmt.hpp"

// ZeroFunction 应该放到Interpreter 类里面
namespace zero {

std::string ZeroFunction::to_string() {
    return std::string("<fn " + declaration->name.lexeme + ">");
}

std::any ZeroFunction::call(Interpreter &interpreter,
                            std::vector<std::any> arguments) {
    // auto env = std::make_unique<Environment>(closure);
    // 创建一个新的环境, 包含全局环境
    auto env = std::make_unique<Environment>(interpreter.environment.get());
    for (auto i = 0u; i < declaration->params.size(); i++) {
        env->define(declaration->params[i].lexeme,
                    arguments[i]); // "a": 1, "b": 2
    }

    try {
        interpreter.execute_block(declaration->body, std::move(env));
    } catch (ZeroReturn &returnValue) {
        return returnValue.value;
    }

    return {};
}

} // namespace zero

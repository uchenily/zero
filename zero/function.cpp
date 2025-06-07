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
    auto env = Environment(interpreter.get_globals());
    for (auto i = 0u; i < declaration->params.size(); i++) {
        env.define(declaration->params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.execute_block(declaration->body, &env);
    } catch (ZeroReturn &returnValue) {
        return returnValue.value;
    }

    return {};
}

std::string NativeFunction::to_string() { return "<native fn>"; }
std::any NativeFunction::call([[maybe_unused]] Interpreter &interpreter,
                              std::vector<std::any> arguments) {
    return native_func(arguments);
}

} // namespace zero

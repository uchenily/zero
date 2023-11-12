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
    auto env = std::make_unique<Environment>(closure);
    for (size_t i = 0; i < declaration->params.size(); i++) {
        env->define(declaration->params[i].lexeme, arguments[i]);
    }

    try {
        interpreter.execute_block(declaration->body, std::move(env));
    } catch (ZeroReturn &returnValue) {
        return returnValue.value;
    }

    return nullptr;
}

} // namespace zero

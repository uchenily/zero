#include "stmt.hpp"

namespace zero {

class Program {
public:
    explicit Program(std::vector<std::unique_ptr<Stmt>> statements)
        : statements_{std::move(statements)} {}

    auto &get_statements() { return statements_; };

private:
    std::vector<std::unique_ptr<Stmt>> statements_;
};
} // namespace zero

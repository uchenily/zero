#pragma once

#include <exception>

namespace zero {

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#define expect(condition)                                                      \
    if (!likely(condition)) {                                                  \
        std::terminate();                                                      \
    }

} // namespace zero

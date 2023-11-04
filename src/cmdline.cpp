#include "cmdline.hpp"

#include "assert.hpp"
#include "fmt/core.h"

#include <cstring>
#include <map>
#include <string>

namespace zero {
namespace CmdLine {

// 前置声明
class Opt;
static Usage g_usage = nullptr;
static std::map<std::string, Opt> g_opts;

enum class OptType {
    INT,
    BOOL,
    STRING,
};

class Opt {
public:
    Opt() = default;
    Opt(std::string *value, std::string default_value, bool required)
        : m_type{OptType::STRING}, m_value{value}, m_required{required} {
        *(std::string *) m_value = std::move(default_value);
    }

    Opt(int *value, int default_value, bool required)
        : m_type{OptType::INT}, m_value{value}, m_required{required} {
        *(int *) m_value = default_value;
    }

    Opt(bool *value, bool default_value, bool required)
        : m_type{OptType::BOOL}, m_value{value}, m_required{required} {
        *(bool *) m_value = default_value;
    }

    void SetValue(std::string value) {
        if (m_type == OptType::STRING) {
            *(std::string *) m_value = std::move(value);
        } else if (m_type == OptType::INT) {
            *(int *) m_value = std::stoi(value);
        }
    }

    void SetValue(bool value) {
        if (m_type == OptType::BOOL) {
            *(bool *) m_value = value;
        } else {
            fmt::print("Invalid OptType\n");
            exit(0);
        }
    }

    bool IsRequired() const { return m_required; }

private:
    OptType m_type;
    void *m_value;
    bool m_required;
};

void StrOptRequired(std::string *value, std::string name) {
    g_opts[std::move(name)] = Opt(value, "", true);
}

void IntOptRequired(int *value, std::string name) {
    g_opts[std::move(name)] = Opt(value, 0, true);
}

void BoolOpt(bool *value, std::string name) {
    g_opts[std::move(name)] = Opt(value, false, false);
}

static int ParseOpt(int argc, char *argv[], int &parse_index) {
    // 解析包括必选和可选参数
    char *opt = argv[parse_index];
    size_t opt_len = strlen(opt);
    expect(opt_len >= 2);
    expect(opt[0] == '-' && opt[1] == '-');
    opt += 2;
    opt_len -= 2;

    // 执行到这里说明是一个选项, 接下来判断这个选项是否已经指定了参数
    bool has_argument = false;
    std::string argument = "";
    for (size_t i = 1; i < opt_len; i++) {
        if (opt[i] == '=') {
            has_argument = true;
            // 取等号之后的内容赋值为argument
            argument = std::string(opt + i + 1);
            // 这样opt指向的字符串就是'='之前的内容
            opt[i] = 0;
            break;
        }
    }

    std::string opt_name = std::string(opt);
    if (opt_name == "help" || opt_name == "h") {
        g_usage();
        parse_index++;
        return 1;
    }

    auto iter = g_opts.find(opt_name);
    if (iter == g_opts.end()) {
        fmt::print("option provided but not defined: {}\n", opt_name);
        return -1;
    }
    // FIXME: 可选, 但是传入了参数
    if (not iter->second.IsRequired()) {
        iter->second.SetValue(true);
        parse_index++;
        return 0;
    }

    // 必选参数
    if (has_argument) {
        parse_index++;
    } else if (parse_index + 1 < argc) {
        has_argument = true;
        argument = std::string(argv[parse_index + 1]);
        parse_index += 2;
    }

    // https://en.cppreference.com/w/cpp/language/operator_alternative
    if (not has_argument) {
        fmt::print("option needs an argument: {}\n", opt_name);
        return -1;
    }
    iter->second.SetValue(argument);
    return 0;
}

void SetUsage(Usage usage) { g_usage = std::move(usage); }

int Parse(int argc, char *argv[]) {
    expect(g_usage != nullptr);

    int parse_index = 1;
    int retval = 0;
    while (parse_index < argc) {
        retval = ParseOpt(argc, argv, parse_index);
        if (retval != 0) {
            return retval;
        }
    }
    return 0;
}
} // namespace CmdLine
} // namespace zero

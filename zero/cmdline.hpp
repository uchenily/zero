#pragma once

#include <functional>
#include <string>

namespace zero {
namespace CmdLine {

using Usage = std::function<void()>;

// 可选参数
void BoolOpt(bool *value, std::string name);
void IntOpt(int *value, std::string name, int default_value);
void StrOpt(std::string *value, std::string name, std::string default_value);
// 位置参数, 不需要 '--', 也属于可选
void StrPositional(std::string *value);

// 必选参数
void IntOptRequired(int *value, std::string name);
void StrOptRequired(std::string *value, std::string name);

void SetUsage(Usage usage);
int Parse(int argc, char *argv[]);
} // namespace CmdLine
} // namespace zero

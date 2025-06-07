#pragma once
#include <fstream>
#include <sstream>
#include <string>

namespace utils {

inline auto file_exists(const std::string &file_path) -> bool {
    std::fstream file(file_path);
    return file.good();
}

inline auto read_file(const std::string &file_path) -> std::string {
    std::fstream file(file_path, std::ios::in | std::ios::binary);
    std::stringstream buf;

    buf << file.rdbuf();

    return buf.str();
};

} // namespace utils

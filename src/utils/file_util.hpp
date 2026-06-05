#pragma once
#ifndef BLUESTONE_UTILS_FILE_UTIL_HPP
#define BLUESTONE_UTILS_FILE_UTIL_HPP

#include <string>
#include <string_view>

namespace bluestone {

class FileUtil {
public:
    // Reads an entire file (like a .pem or .key) into a std::string securely
    static std::string read_key_file(std::string_view filepath);
};

} // namespace bluestone

#endif // BLUESTONE_UTILS_FILE_UTIL_HPP

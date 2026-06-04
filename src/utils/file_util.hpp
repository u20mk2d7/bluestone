#pragma once
#ifndef BLUESTONE_UTILS_FILE_UTIL_HPP
#define BLUESTONE_UTILS_FILE_UTIL_HPP

#include <string>

namespace bluestone {

  class FileUtil {
   public:
    static std::string read_key_file(std::string_view file_path);
  };

}  // namespace bluestone

#endif  // BLUESTONE_UTILS_FILE_UTIL_HPP

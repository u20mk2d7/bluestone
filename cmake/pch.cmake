# cmake/pch.cmake

add_library(bluestone_pch INTERFACE)

target_precompile_headers(bluestone_pch INTERFACE
  <cstdint>
  <cstddef>
  <limits>
  <vector>
  <map>
  <unordered_map>
  <array>
  <list>
  <string>
  <string_view>
  <format>
  <algorithm>
  <utility>
  <memory>
  <functional>
  <tuple>
  <new>
  <thread>
  <mutex>
  <chrono>
  <fstream>
  <filesystem>
  <iostream>
  <regex>
  <sstream>
  <set>
  <unordered_set>
  <optional>
  <variant>
  <atomic>
  <condition_variable>
  <type_traits>
  <future>
)

message(STATUS "Precompiled Headers (PCH) initialized.")

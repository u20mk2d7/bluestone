# cmake/pch.cmake

add_library(bluestone_pch INTERFACE)

target_precompile_headers(bluestone_pch INTERFACE
  <algorithm>
  <functional>
  <memory>
  <regex>
  <iostream>
  <fstream>
  <sstream>
  <array>
  <map>
  <set>
  <string>
  <string_view>
  <unordered_map>
  <unordered_set>
  <vector>
  <optional>
  <tuple>
  <utility>
  <variant>
  <atomic>
  <condition_variable>
  <future>
  <mutex>
  <thread>
)

message(STATUS "Precompiled Headers (PCH) initialized.")

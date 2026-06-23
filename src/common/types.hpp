#pragma once
#ifndef COMMON_TYPES_HPP
#define COMMON_TYPES_HPP
#include <iostream>
#include <type_traits>
#include <cstdint>

namespace bluestone {
struct NormalizeTick {
  uint64_t gateway_recv_tsc;  // 8 bytes Hardware Timestamp for latency tracking

  uint64_t timestamp;      // 8 bytes
  uint64_t instrument_id;  // 8 bytes
  double bid_price;        // 8 bytes
  double ask_price;        // 8 bytes
  uint32_t bid_qty;        // 4 bytes
  uint32_t ask_qty;        // 4 bytes
};

// 2. The Compile-Time Safety Guardrail
// This guarantees that NormalizeTick never accidentally slows down your
// lock-free queue.
static_assert(std::is_trivially_copyable<NormalizeTick>::value,
              "NormalizeTick must be trivially copyable!");

}  // namespace bluestone


#endif  // COMMON_TYPES_HPP

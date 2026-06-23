#pragma once
#ifndef BLUESTONE_CORE_MARKETDATA_ORDER_BOOK_HPP
#define BLUESTONE_CORE_MARKETDATA_ORDER_BOOK_HPP
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "absl/container/flat_hash_map.h"

namespace bluestone {
  namespace marketdata {

    // 1. Enums strictly sized for cache-line packing
    enum class Side : uint8_t { Buy = 0, Sell = 1 };

    // 2. The Flat Order representation
    struct Order {
      uint64_t order_id;
      int64_t price_ticks;  // Price represented in integer ticks
      uint64_t size;
      Side side;
    };

    // 3. Price Level representation (Tracks total volume at a specific price)
    struct PriceLevel {
      int64_t price_ticks = 0;
      uint64_t total_volume = 0;
      uint32_t order_count = 0;
    };

    // 4. Best Bid/Offer (BBO) representation for the Strategy Engine
    struct BBO {
      int64_t best_bid = 0;
      uint64_t bid_size = 0;
      int64_t best_ask = std::numeric_limits<int64_t>::max();
      uint64_t ask_size = 0;
    };

    class OrderBook {
      private:
      // Re-evaluates the BBO if a top-of-book order is modified/canceled
      void update_bbo(Side side) noexcept;

      // --- State Variables Best Bid/Offer ---
      BBO current_bbo_;

      // --- The Memory Arenas ---
      // Maps OrderID -> Order object (O(1) lookup)
      absl::flat_hash_map<uint64_t, Order> order_map_;

      // Maps Price (in ticks) -> PriceLevel object (O(1) lookup)
      absl::flat_hash_map<int64_t, PriceLevel> bid_levels_;
      absl::flat_hash_map<int64_t, PriceLevel> ask_levels_;

     public:
      // Boot sequence: Pre-allocate everything so zero allocations happen in
      // the hot path
      explicit OrderBook(size_t max_expected_orders = 1000000,
                         size_t max_price_levels = 50000);
      ~OrderBook() = default;

      // --- The Hot Path (L1 Cache Optimized) ---
      void add_order(uint64_t order_id, int64_t price, uint64_t size,
                     Side side) noexcept;
      void modify_order(uint64_t order_id, uint64_t new_size) noexcept;
      void cancel_order(uint64_t order_id) noexcept;

      // Direct memory access to the current BBO (Zero copy)
      [[nodiscard]] inline const BBO& get_bbo() const noexcept {
        return current_bbo_;
      }

    };

  }  // namespace marketdata
}  // namespace bluestone

#endif  // BLUESTONE_CORE_MARKETDATA_ORDER_BOOK_HPP

/*
Why this design crushes the competition:
The noexcept Guarantee: Notice how every hot-path function is marked noexcept.
This tells the LLVM compiler to completely strip out stack-unwinding code and
exception-handling overhead, making the binary drastically smaller and faster.

absl::flat_hash_map: Standard std::unordered_map allocates memory nodes
scattered randomly across your RAM, causing cache misses. flat_hash_map stores
everything in a contiguous block of memory. When your CPU cache grabs an order,
it automatically pre-fetches the adjacent ones.

Inline BBO Getter: The get_bbo() function is strictly inline and returns a const
reference. The Risk Engine and Strategy components can instantly read the exact
Best Bid and Ask in 0 CPU cycles—it's literally just a memory pointer.

But before I do, how do you want to handle order tracking? In a true matching
engine, we need to know the exact queue position (FIFO priority). Are we
building this Order Book purely as a Market Data Replica (just tracking exchange
prices) or as an Internal Matching Engine (simulating queue positions for
backtesting)?

*/

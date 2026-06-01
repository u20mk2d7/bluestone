#pragma once
#ifndef TRADE_QUEUE_HPP
#define TRADE_QUEUE_HPP

#include <array>
#include <boost/lockfree/spsc_queue.hpp>

namespace bluestone {

  // 1. The ultra-fast, fixed-size memory struct (Zero Heap Allocation)
  struct TradeEvent {
    std::array<char, 16> symbol;
    double price;
    double quantity;
    uint64_t timestamp_ns;
  };

  // 2. The Lock-Free Ring Buffer (10,000 slots pre-allocated in RAM)
  using TradeQueue =
      boost::lockfree::spsc_queue<bluestone::TradeEvent,
                                  boost::lockfree::capacity<10000>>;

}  // namespace bluestone

#endif  // TRADE_QUEUE_HPP

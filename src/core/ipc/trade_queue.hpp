#pragma once
#ifndef TRADE_QUEUE_HPP
#define TRADE_QUEUE_HPP

#include <array>
#include <boost/lockfree/spsc_queue.hpp>
#include <new>  // for std::hardware_destructive_interference_size

#include "common/types.hpp"

namespace bluestone {

  /*
   * Single-Producer, Single-Consumer
    The using keyword creates a clean alias (a nickname). Instead of typing out
    the massive, ugly boost::lockfree::spsc_queue<bluestone::NormalizeTick,
    boost::lockfree::capacity<10000>> every time you want to use it, you can now
    just type bluestone::TradeQueue.
   */
  // The Lock-Free Ring Buffer (capacity 10,000 slots pre-allocated in RAM)
  using TradeQueue =
      boost::lockfree::spsc_queue<bluestone::NormalizeTick,
                                  boost::lockfree::capacity<10000>>;
}  // namespace bluestone

#endif  // TRADE_QUEUE_HPP

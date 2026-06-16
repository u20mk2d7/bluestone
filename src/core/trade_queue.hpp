#pragma once
#ifndef TRADE_QUEUE_HPP
#define TRADE_QUEUE_HPP

#include <array>
#include <boost/lockfree/spsc_queue.hpp>

#include "common/types.hpp"

namespace bluestone {

  // The Lock-Free Ring Buffer (10,000 slots pre-allocated in RAM)
  using TradeQueue =
      boost::lockfree::spsc_queue<bluestone::NormalizeTick,
                                  boost::lockfree::capacity<10000>>;
}  // namespace bluestone

#endif  // TRADE_QUEUE_HPP

//
#include "trade_engine.hpp"

#include "core/ipc/trade_queue.hpp"
//
#include <atomic>
#include <iostream>
#include <memory>
#include <utility>

// Add this block near the top of trade_engine.cpp with your other includes:
#if defined(__x86_64__) || defined(_M_X64)
#include <emmintrin.h>  // Required for _mm_pause()
#endif

namespace bluestone {
  DataGatewayEngine::DataGatewayEngine(
      std::unique_ptr<bluestone::IExchangeConnector> exchange,
      std::shared_ptr<bluestone::TradeQueue> queue)
      : exchange_(std::move(exchange)),
        queue_(std::move(queue)),
        running_(false) {
    std::cout << "[DataGatewayEngine] Constructure....\n";
  }

  //
  void DataGatewayEngine::run() {
    std::cout << "[DataGatewayEngine]: Running Exchange Connection...\n";
    exchange_->connect();
    std::cout << "[DataGatewayEngine]: Engaging HFT Nanosecond Loop...\n";
    running_.store(true, std::memory_order_release);

    while (running_.load(std::memory_order_acquire)) {
      bluestone::NormalizeTick tick;
      // pop() is lock-free. It returns instantly without asking the OS for a
      // mutex lock.
      if (queue_->pop(tick)) {
        // This code executes within single-digit nanoseconds of the Gateway
        // pushing it.
        //
        std::cout << "[TICK] InstID: " << tick.instrument_id
                  << " | Bid: " << tick.bid_price << " (" << tick.bid_qty << ")"
                  << " | Ask: " << tick.ask_price << " (" << tick.ask_qty
                  << ")\n";
      } else {
// QUEUE IS EMPTY. DO NOT SLEEP.
// We gently yield the CPU cache line to let the Gateway thread write to
// it.
#if defined(__x86_64__) || defined(_M_X64)
        _mm_pause();  // Intel/AMD hardware pause
#elif defined(__aarch64__)
        // Apple Silicon / ARM hardware yield
        asm volatile("yield" ::: "memory");
#endif
      }
    }
  }

  void DataGatewayEngine::stop() {
    std::cout << "[DataGatewayEngine]: Stopping Engine...\n";
    running_.store(false, std::memory_order_release);  // Safely breaks the loop
    exchange_->disconnect();
  }

}  // namespace bluestone

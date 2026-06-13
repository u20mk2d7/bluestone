#include "trade_engine.hpp"

#include <iostream>
#include <memory>
#include <utility>

#include "trade_queue.hpp"

namespace bluestone {
  DataGatewayEngine::DataGatewayEngine(
      std::unique_ptr<bluestone::IExchangeConnector> exchange,
      std::shared_ptr<bluestone::TradeQueue> queue)
      : exchange_(std::move(exchange)),
        queue_(std::move(queue)),
        running_(false) {}

  void DataGatewayEngine::run() {
    std::cout << "[DataGatewayEngine]: Stopping Exchange Connection...\n";
    exchange_->connect();
    std::cout << "[DataGatewayEngine]: Engaging HFT Nanosecond Loop...\n";
    running_.store(true, std::memory_order_release);

    while (running_.load(std::memory_order_acquire)) {
      bluestone::TradeEvent event;
      // pop() is lock-free. It returns instantly without asking the OS for a
      // mutex lock.
      if (queue_->pop(event)) {
        // This code executes within single-digit nanoseconds of the Gateway
        // pushing it.
        std::cout << "[TICK] Symbol: " << event.symbol.data()
                  << " | Price: " << event.price << " | Qty: " << event.quantity
                  << "\n";

      } else {
        // QUEUE IS EMPTY. DO NOT SLEEP.
        // We gently yield the CPU cache line to let the Gateway thread write to
        // it.
#if defined(__x86_64__) || defined(_M_X64)
        _mm_pause();  // Intel/AMD hardware pause
#elif defined(__aarch64__)
        asm volatile("yield" ::
                         : "memory");  // Apple Silicon / ARM hardware yield
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

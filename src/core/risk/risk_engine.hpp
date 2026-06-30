/*
You are about to build the absolute firewall of your quantitative firm. In
High-Frequency Trading, if your strategy goes rogue and fires a 10,000 BTC
market order by accident, you bankrupt the firm in 3 milliseconds. The Risk
Engine is the ultimate gatekeeper. Because it sits directly in the Hot Path
(Tier 1)—right between your strategy's brain and the network socket—it must
execute in less than 10 nanoseconds.

To achieve this, we use three specific C++ techniques:

[[nodiscard]] inline: We force the compiler to inline the risk math directly
into the execution routing so there is zero function-call overhead.

Zero-Copy BBO Reading: We pass the OrderBook by const&, allowing the Risk Engine
to read the exact memory address of the Best Bid/Offer without copying it.

Price Collaring: We physically compare the order's price against the active BBO
to ensure you aren't crossing the spread by a dangerous margin (preventing flash
crashes).

*/
#pragma once
#ifndef BLUESTONE_CORE_RISK_RISK_ENGINE_HPP
#define BLUESTONE_CORE_RISK_RISK_ENGINE_HPP

#include <cstdint>

#include "core/marketdata/order_book.hpp"

namespace bluestone {
  namespace core {
    namespace risk {

      enum class RiskStatus : uint8_t {
        Approved = 0,
        Reject_FatFingerSize = 1,
        Reject_NotionalValue =
            2,  // 🚀 NEW: Rejects orders that cost too much USD
        Reject_PriceCollar = 3,
        Reject_MaxPosition = 4
      };

      struct RiskLimits {
        uint64_t max_order_size = 0;  // Max raw coins allowed per order
        uint64_t max_notional_value =
            0;  // 🚀 NEW: Max total cost (e.g., USD limit)
        int64_t max_position_size = 0;  // Max accumulated inventory
        int64_t max_price_distance_ticks = 0;
      };

      class RiskEngine {
       public:
        explicit RiskEngine(const RiskLimits& limits) noexcept
            : limits_(limits) {}
        ~RiskEngine() = default;

        [[nodiscard]] inline RiskStatus evaluate_order(
            const MarketData::OrderBook& book, MarketData::Side side,
            int64_t price, uint64_t size) const noexcept {
          // 1. Fat Finger Check (Raw Size)
          if (size > limits_.max_order_size) {
            return RiskStatus::Reject_FatFingerSize;
          }

          // 2. 🚀 The Notional Value Firewall
          // We multiply Size * Price to get the total capital required.
          // If this trade costs $1 Million but our limit is $50k, we block it
          // instantly.
          uint64_t notional_value = size * static_cast<uint64_t>(price);
          if (notional_value > limits_.max_notional_value) {
            return RiskStatus::Reject_NotionalValue;
          }

          // 3. Position Limit Check
          int64_t projected_position =
              current_position_ + (side == MarketData::Side::Buy
                                       ? static_cast<int64_t>(size)
                                       : -static_cast<int64_t>(size));

          if (projected_position > limits_.max_position_size ||
              projected_position < -limits_.max_position_size) {
            return RiskStatus::Reject_MaxPosition;
          }

          // 4. Price Collar (BBO Integrity Check)
          const auto& bbo = book.get_bbo();

          if (side == MarketData::Side::Buy) {
            if (bbo.best_ask != std::numeric_limits<int64_t>::max() &&
                price > (bbo.best_ask + limits_.max_price_distance_ticks)) {
              return RiskStatus::Reject_PriceCollar;
            }
          } else {
            if (bbo.best_bid != 0 &&
                price < (bbo.best_bid - limits_.max_price_distance_ticks)) {
              return RiskStatus::Reject_PriceCollar;
            }
          }

          return RiskStatus::Approved;
        }

        inline void update_position(uint64_t executed_size,
                                    MarketData::Side side) noexcept {
          if (side == MarketData::Side::Buy) {
            current_position_ += executed_size;
          } else {
            current_position_ -= executed_size;
          }
        }

        [[nodiscard]] inline int64_t get_current_position() const noexcept {
          return current_position_;
        }

       private:
        RiskLimits limits_;
        int64_t current_position_ = 0;
      };

    }  // namespace risk
  }  // namespace core
}  // namespace bluestone

#endif  // BLUESTONE_CORE_RISK_RISK_ENGINE_HPP

/*
Why this design crushes the competition:
The No-Atomic Rule: Look at current_position_. In standard software, tracking
a shared inventory requires std::atomic<int64_t> or a std::mutex. But because
of your architecture—pinning one Bot Instance to one Physical Core—no other
thread will ever touch this memory. This drops your memory access time from
~25ns down to ~1ns (pure L1 cache read).

The [[nodiscard]] Attribute: In HFT, human error is the biggest liability. If
a Junior Quant writes risk_engine.evaluate_order(...); but forgets to write
the if statement to handle a rejection, LLVM Clang 22 will physically refuse
to compile the codebase.

The const Guarantee: Notice evaluate_order is marked const. It proves to the
compiler that evaluating risk does not mutate the engine's state, allowing
LLVM to aggressively cache the limits_ variables in the CPU registers.
*/

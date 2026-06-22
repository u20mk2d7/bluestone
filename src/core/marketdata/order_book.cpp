#include "order_book.hpp"

#include <algorithm>

namespace bluestone {
  namespace marketdata {

    // --- BOOT SEQUENCE (Cold Path) ---
    bluestone::marketdata::OrderBook::OrderBook(size_t max_expected_orders,
                                                size_t max_price_levels) {
      // 🚀 THE PRE-ALLOCATION SHIELD
      // By reserving this memory on boot, we guarantee that inserting orders
      // during a volatile market spike will NEVER trigger a heap allocation or
      // rehash.
      order_map_.reserve(max_expected_orders);
      bid_levels_.reserve(max_price_levels);
      ask_levels_.reserve(max_price_levels);
    }

    // --- THE HOT PATH (Nanosecond Execution) ---

    void OrderBook::add_order(uint64_t order_id, int64_t price, uint64_t size,
                              Side side) noexcept {
      // 1. Store the raw order in O(1) time
      order_map_.insert_or_assign(order_id, Order{order_id, price, size, side});

      // 2. Update the Price Level aggregation
      auto& level_map = (side == Side::Buy) ? bid_levels_ : ask_levels_;
      auto& level = level_map[price];  // Creates it if it doesn't exist (memory
                                       // is pre-reserved!)

      level.price_ticks = price;
      level.total_volume += size;
      level.order_count++;

      // 3. Ultra-fast BBO Check (Only update if it improves the current best
      // price)
      if (side == Side::Buy) {
        if (price > current_bbo_.best_bid) {
          current_bbo_.best_bid = price;
          current_bbo_.bid_size = level.total_volume;
        } else if (price == current_bbo_.best_bid) {
          current_bbo_.bid_size = level.total_volume;
        }
      } else {  // Sell
        if (price < current_bbo_.best_ask) {
          current_bbo_.best_ask = price;
          current_bbo_.ask_size = level.total_volume;
        } else if (price == current_bbo_.best_ask) {
          current_bbo_.ask_size = level.total_volume;
        }
      }
    }

    void OrderBook::modify_order(uint64_t order_id,
                                 uint64_t new_size) noexcept {
      // O(1) pointer jump to the order
      auto it = order_map_.find(order_id);
      if (it == order_map_.end()) return;  // Ghost order drop

      Order& order = it->second;
      int64_t delta =
          static_cast<int64_t>(new_size) - static_cast<int64_t>(order.size);
      order.size = new_size;

      // Update the aggregated volume at that specific price level
      auto& level_map = (order.side == Side::Buy) ? bid_levels_ : ask_levels_;
      auto& level = level_map[order.price_ticks];
      level.total_volume += delta;

      // Direct BBO Size patch if this order sits at the top of the book
      if (order.side == Side::Buy &&
          order.price_ticks == current_bbo_.best_bid) {
        current_bbo_.bid_size = level.total_volume;
      } else if (order.side == Side::Sell &&
                 order.price_ticks == current_bbo_.best_ask) {
        current_bbo_.ask_size = level.total_volume;
      }
    }

    void OrderBook::cancel_order(uint64_t order_id) noexcept {
      auto it = order_map_.find(order_id);
      if (it == order_map_.end()) return;

      const Order order =
          it->second;  // Copy intentionally to safely erase next
      order_map_.erase(it);

      auto& level_map = (order.side == Side::Buy) ? bid_levels_ : ask_levels_;
      auto level_it = level_map.find(order.price_ticks);

      if (level_it != level_map.end()) {
        level_it->second.total_volume -= order.size;
        level_it->second.order_count--;

        // If that was the last order at this price, the price level vanishes
        if (level_it->second.order_count == 0) {
          level_map.erase(level_it);

          // ⚠️ THE DROP OF DEATH
          // If the price level that just vanished was our Best Bid or Best Ask,
          // we must scan the book to find the *new* Best Bid/Ask.
          if ((order.side == Side::Buy &&
               order.price_ticks == current_bbo_.best_bid) ||
              (order.side == Side::Sell &&
               order.price_ticks == current_bbo_.best_ask)) {
            update_bbo(order.side);
          }
        } else {
          // Level survives, just patch the BBO size if needed
          if (order.side == Side::Buy &&
              order.price_ticks == current_bbo_.best_bid) {
            current_bbo_.bid_size = level_it->second.total_volume;
          } else if (order.side == Side::Sell &&
                     order.price_ticks == current_bbo_.best_ask) {
            current_bbo_.ask_size = level_it->second.total_volume;
          }
        }
      }
    }

    // --- INTERNAL STATE RECOVERY ---
    void OrderBook::update_bbo(Side side) noexcept {
      if (side == Side::Buy) {
        current_bbo_.best_bid = 0;
        current_bbo_.bid_size = 0;

        // Scan the hash map to find the new highest bid
        for (const auto& [price, level] : bid_levels_) {
          if (price > current_bbo_.best_bid) {
            current_bbo_.best_bid = price;
            current_bbo_.bid_size = level.total_volume;
          }
        }
      } else {
        current_bbo_.best_ask = std::numeric_limits<int64_t>::max();
        current_bbo_.ask_size = 0;

        // Scan the hash map to find the new lowest ask
        for (const auto& [price, level] : ask_levels_) {
          if (price < current_bbo_.best_ask) {
            current_bbo_.best_ask = price;
            current_bbo_.ask_size = level.total_volume;
          }
        }
      }
    }

  }  // namespace marketdata
}  // namespace bluestone

/*
The Architect's Warning: "The Drop of Death"I need you to look closely at the
update_bbo() function at the bottom.Adding an order is < 50 nanoseconds because
it relies strictly on pointer arithmetic. However, if someone cancels an order
that wipes out the absolute top of the book (Best Bid or Best Ask), your engine
has to iterate through the absl::flat_hash_map to find the new best
price.Because a hash map is unordered, finding the highest/lowest value requires
an $O(N)$ scan. Right now, for crypto markets with a few hundred active price
levels, this will still execute in under a microsecond (well within your Tier 1
budget).But if we start trading highly liquid assets on IBKR or Coinbase where
there are tens of thousands of price levels, that $O(N)$ loop will cause a
micro-stutter. When you are ready for Phase 2 of this book, I'll show you how to
replace absl::flat_hash_map with a custom Direct-Mapped Array (an array indexed
physically by the price tick), which completely eliminates loops and turns
update_bbo() back into an $O(1)$ constant time lookup.
*/

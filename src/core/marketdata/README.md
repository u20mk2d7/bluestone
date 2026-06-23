The HFT Order Book: Architectural Blueprint

As the Director of your quant infrastructure, I need you to understand that our Order Book is not a database. It is a Live Memory Mirror. Its only job is to look exactly like the exchange's order book, but sit inside our CPU's L1 Cache so our strategy can read it in 0 nanoseconds.

Here is exactly why every class, struct, and function exists.

1. The Data Types (The "Nouns")

We don't use large, bulky classes for our data. We use raw, flat structs.

Why do we use Integer "Ticks" instead of double?

If Binance says the price of BTC is $65,123.45, a junior developer saves it as a double.
Why we don't: Floating-point math is done on the FPU (Floating Point Unit) of the processor. It takes extra cycles and introduces rounding errors.
Our Solution: We multiply the price by a fixed factor (like 100) and save it as an int64_t (e.g., 6512345). Integer math executes instantly on the CPU's main ALU.

The Structs

enum class Side: A 1-byte flag (0 for Buy, 1 for Sell).

struct Order: Tracks an individual resting order (ID, Price, Size, Side).

struct PriceLevel: We group orders at the same price together. If 10 people want to buy BTC at $65,000, the StrategyEngine only cares that there is a total of 10 BTC available at that level.

struct BBO (Best Bid & Offer): This is the most important struct. The strategy rarely cares about the 50th price level down. It needs to know the absolute Top of the Book instantly.

2. The Memory Containers (The "Bookshelf")

We need to store millions of Order and PriceLevel structs.

Why we do NOT use std::map or std::unordered_map

Standard C++ maps dynamically allocate memory (new or malloc) every time a new order arrives. In a flash crash, Binance will send you 10,000 orders in a millisecond. If you call malloc 10,000 times, the Linux kernel will pause your thread, and you will miss the trade.

Our Solution: absl::flat_hash_map

We use Google's Abseil library. A flat_hash_map allocates one massive, contiguous chunk of memory once during boot-up.
When an order arrives, we just slot it into the pre-allocated array.

Result: O(1) constant-time lookups with Zero Allocations in the hot path.

3. The Core Functions (The "Verbs")

The Cold Path (Constructor)

OrderBook(max_orders, max_levels): When the bot boots up, this function runs. It calls .reserve(max_orders) on our hash maps. We physically pre-allocate the RAM before we even connect to the internet.

The Hot Path (add_order, modify_order, cancel_order)

Why they exist: LMAX and Binance don't send you a full picture of the book every nanosecond; they send you deltas (changes). "Add 5 BTC here, cancel 2 BTC there." These functions apply those changes to our memory mirror.

Why they are marked noexcept: Standard C++ generates hidden "stack-unwinding" code just in case a function throws an exception. By adding noexcept, we promise the compiler we will never throw an exception. The compiler rewards us by stripping out the safety code, making the assembly instructions drastically smaller and faster.

The Accessor (get_bbo())

Why it exists: The Strategy Engine needs to look at the Best Bid and Best Ask to decide whether to fire a trade.

Why it is marked [[nodiscard]] inline const BBO&: * inline: Replaces the function call directly with the memory address. Zero function-call overhead.

const BBO&: We return a Reference. We do not copy the data. We just hand the strategy a direct read-only pointer to the exact physical memory where the top-of-book sits.

4. The Necessary Evil (update_bbo())

The "Drop of Death": If the Best Bid is $65,000, and someone cancels that exact order, the price drops. But what is the new Best Bid? Is it $64,999? Is it $64,950?
Because we use a hash map (which is unsorted), we don't immediately know what the next highest number is.

Why it exists: update_bbo() scans the hash map to find the new absolute best price.

The Tradeoff: This takes $O(N)$ time. In Crypto (a few hundred price levels), it's fine. In TradFi equities (tens of thousands of levels), we would have to replace the hash map with a Direct-Mapped Array to avoid this scan.

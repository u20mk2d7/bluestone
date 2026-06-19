# Algorithmic Trading Engine Architecture & Execution Plan

## Summary

* **The Antennas (Gateways):** You have a pure C++ QuickFIX engine connected to LMAX for institutional fiat liquidity and a Boost.Beast WebSocket engine ingesting Binance JSON streams via zero-copy `simdjson`.
* **The Nervous System (IPC Bus):** You have physically isolated your network threads from your strategy threads using a lock-free Single Producer Single Consumer (SPSC) ring buffer pre-allocated in RAM.
* **The Brain (Core Engine):** You have a dedicated strategy thread pinned to an isolated CPU core, polling the memory bus via cache-line yielding (`_mm_pause`) to avoid kernel context switches.

---

## TODO List

### 📋 PHASE 1: Hardware Empathy & The Engineering Proof
*Before we write trading algorithms, you must prove your engine's speed to the nanosecond.*

#### Task 1.1: TSC Latency Proving (Immediate)
* **Objective:** Instrument the `NormalizeTick` struct with hardware timestamps to prove your inter-thread latency.
* **Action:** Utilize the `__rdtscp` intrinsic inside `tsc_clock.hpp` to stamp the exact CPU cycle the packet leaves the network socket. Stamp it again the moment `DataGatewayEngine` pops it from the queue.
* **Deliverable:** Terminal output proving your lock-free queue transit time is under 200 nanoseconds.

#### Task 1.2: Order Book "Drop of Death" Optimization
* **Objective:** Eliminate the $O(N)$ hash map scan in your L2 Order Book.
* **Action:** Your current `OrderBook` uses `absl::flat_hash_map`. As noted in the architecture, if a cancel order wipes out the Top of Book, your engine iterates the map to find the new BBO (Best Bid/Offer). You must rewrite the `PriceLevel` aggregation using a **Direct-Mapped Array** (an array physically indexed by the integer price tick) to guarantee $O(1)$ constant-time BBO updates, perfectly sized to fit inside the CPU L1 cache.

---

### 📋 PHASE 2: The Alpha Generation
*Speed is irrelevant without edge. We will implement a Lead-Lag Statistical Arbitrage model.*

#### Task 2.1: The Unified Order Book Fusion
* **Objective:** Merge the TradFi and Crypto data streams into a single worldview.
* **Action:** Instantiate two `OrderBook` objects inside `DataGatewayEngine`. As `NormalizeTick` objects fly off the queue, route Instrument A (Binance BTC/USDT) to Book A, and Instrument B (LMAX BTC/USD) to Book B.

#### Task 2.2: The Lead-Lag Trigger
* **Objective:** Detect microsecond pricing inefficiencies.
* **Action:** Binance retail volume heavily leads institutional LMAX pricing. Write an algorithm that monitors the `get_bbo()` of both books. If Binance's Best Bid spikes above LMAX's Best Ask, the Alpha Engine instantly signals a Buy order to LMAX.

---

### 📋 PHASE 3: The Shield & The Spear
*When the Alpha Engine fires, we must enforce strict risk limits before the packet hits the wire.*

#### Task 3.1: The Zero-Copy Risk Firewall
* **Objective:** Prevent catastrophic fat-finger bankruptcies in under 10 nanoseconds.
* **Action:** Wire the `RiskEngine` directly into the hot path. When the Alpha Engine signals a trade, pass the size and price to `evaluate_order()`. Ensure the `max_notional_value` and `max_price_distance_ticks` limits dynamically block the trade if the spread is too wide.

#### Task 3.2: The Fast FIX Execution Router
* **Objective:** Bypass the standard QuickFIX serialization overhead for outbound trades.
* **Action:** Connect the approved signal to the `FastFixEncoder` to write the `35=D` (NewOrderSingle) payload directly into a pre-allocated raw `char[]` buffer. Push this buffer to a dedicated Execution Gateway thread pinned to a separate core.

---

### 📋 PHASE 4: Operating System Dark Arts (Production Deployment)
*To achieve ultimate determinism on an AlmaLinux production server, we must strip the OS of its power to interrupt your threads.*

#### Task 4.1: Kernel Tuning & CPU Isolation
* **Objective:** Prevent the Linux scheduler from causing micro-stutters.
* **Action:** Modify the GRUB bootloader on your AlmaLinux environment. Add the `isolcpus=2,4,6` parameter to completely hide those cores from the Linux scheduler. Update your `EventLoopWorker`'s `pthread_setaffinity_np` logic to permanently map your Gateways and Strategy engines to those hidden, silent cores.

#### Task 4.2: Power Management Stabilization
* **Objective:** Disable CPU power-saving transitions to eliminate frequency scaling latency.
* **Action:** Disable CPU C-States in the BIOS and enforce the `performance` CPU governor to prevent frequency scaling jitter.
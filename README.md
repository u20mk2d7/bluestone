# 🏛️ Project Bluestone: High-Frequency Trading Architecture Blueprint

**Objective:** To engineer a low-latency, institutional-grade Quantitative Trading Infrastructure bridging Traditional Finance (TradFi via IBKR, FIX)
Websocket Cryptocurrency markets (Binance, OKX, Coinbase). The system is built on modern C++23, strictly adhering to zero-allocation hot paths, lock-free concurrency, and hardware-level thread affinity.

---

## ⚠️ ENGINEERING DIRECTIVES: STRICT CODING STANDARDS
All engineers (and AI Assistants) contributing to Project Bluestone must strictly adhere to the following constraints:
* **Language:** Pure C++23 or latest standard (Standard Template Library only in the core).
* **Style & Linting:** Strict adherence to LLVM coding standards, the Google C++ Style Guide, and C++ Core Guidelines.
* **Architecture:** Uncle Bob's Clean Code principles, SOLID design, and appropriate Gang of Four (GoF) Design Patterns.
* **The Golden Performance Rule:** **Zero** heap allocations (`new`/`malloc`), **zero** mutex locks, and **zero** `std::string` copies in the Hot Path (Tier 1) after the initial boot sequence.

---

## ⏱️ The Latency Horizons (Execution Tiers)

### Tier 1: The Hot Path (1 - 100 Nanoseconds)
* **Purpose:** The Core Trading Engine. This is where market data is parsed, the Order Book is maintained, and "Buy/Sell" decisions are executed.
* **Technology:** Pure C++23 running in main RAM and L1/L2 CPU Cache. Utilizes `simdjson` for zero-allocation parsing and `absl::flat_hash_map` for ultra-fast Order Book lookups.
* **Hardware Execution:** Runs on a dedicated `EventLoopWorker` thread permanently pinned to an isolated physical CPU core. It spins at 100% CPU capacity (busy-wait).

### Tier 2: The Warm Path (100 - 200 Microseconds)
* **Purpose:** Shared State & Global Risk Management. Allows multiple independent bot instances to share global position limits and calculate cross-exchange arbitrage.
* **Technology:** **Valkey (KeyDB)** running completely asynchronously via Unix Domain Sockets (UDS).

### Tier 3: The Cold Path (20 - 50 Milliseconds)
* **Purpose:** The Historian. Purely for auditing, PnL calculations, tax records, and saving permanent receipts.
* **Technology:** **PostgreSQL** database fronted by a **PostgREST** proxy.
* **The Bridge:** Tier 1 sends data to Tier 3 using a lock-free mailbox (`boost::lockfree::spsc_queue`). A background logger thread processes this queue, ensuring the hot path never waits for disk I/O.

### Tier 4: The Management Plane (Seconds)
* **Purpose:** Web Dashboards, Python quantitative monitoring scripts, and User Interfaces.
* **The Firewall:** Tier 4 applications are **strictly banned** from communicating with the C++ trading engine (Tiers 1 & 2). They may only query the PostgreSQL database in Tier 3.

---

## 🌍 Infrastructure & Dependencies

Project Bluestone utilizes a hermetically sealed Conan v2 build system to prevent ABI mismatches and ensure portability.

1. **QuickFIX & OpenSSL:** For secure, encrypted FIX protocol connections (Coinbase/TradFi).
2. **TWSAPI & Protobuf:** Vendored locally and built from source for Interactive Brokers.
3. **Abseil & Boost:** For flat hash maps, CLI parsing, and lock-free queues.
4. **simdjson:** For zero-allocation WebSocket JSON parsing.
5. **Valkey & PostgreSQL:** For external state management.

---

## 📂 Monorepo Directory Structure (The Unified Tree)

```text
bluestone/
├── CMakeLists.txt                 # Master Build File (LTO, march=native, -O3)
├── .env/
│   └── *.json                     # Instance configurations loaded at startup
├── build/                         # Release build output
└── src/
    ├── CMakeLists.txt
    ├── pch.hpp                    # Precompiled headers for fast compilation
    ├── common/
    │   ├── types.hpp              # Flat structs (No std::string in hot path)
    │   └── constants.hpp
    ├── utils/                     # System Utilities (Third-party allowed)
    │   ├── cfg_util.hpp           # JSON config parsers
    │   ├── cli_util.hpp           # Abseil command line parsers
    │   └── event_loop_worker.hpp  # Thread pinning & Asio contexts
    ├── core/                      # The Brain (Pure C++ / STL Only)
    │   ├── utils/                 # HFT Utilities (lockfree queues, memory arenas, timing)
    │   ├── exchange/              # Abstract pure virtual interfaces (i_exchange_connector)
    │   ├── fix/                   # Optimized QuickFIX wrappers & Core FIX logic
    │   ├── tws/                   # TWSAPI low-latency wrapper
    │   ├── strategy/              # Lock-free strategy modules
    │   ├── risk/                  # Ultra-fast pre-check risk engine
    │   ├── marketdata/            # Lock-free order book & tick processor
    │   ├── execution/             # Smart order routing
    │   └── trade_engine.hpp       # Core Engine definition
    ├── gateways/                  # Exchange-Specific Adapters
    │   ├── binance/               # WS & FIX implementations
    │   ├── coinbase/              # FIX implementation
    │   ├── okx/                   # WS implementation
    │   └── interactivebrokers/    # TWSAPI implementation
    └─── main.cpp               # Dynamic executable entry point

```

---

## ⚡ Critical Techniques for Sub-Microsecond Performance

1. **Minimize Allocations & Copying**
* Use custom memory pool allocators. Zero `new` or `malloc` calls after initialization.
* Replace `std::string` in hot paths with `std::string_view` or fixed-size `char` arrays.
* Parse FIX and JSON messages strictly in-place (Zero-Copy).


2. **Lock-Free Everything**
* Utilize `boost::lockfree::spsc_queue` for thread messaging.
* Maintain a lock-free order book and pre-allocated risk limit structures.


3. **Threading & CPU Affinity**
* Pin each bot instance to specific physical CPU cores using OS-level affinity (`sched_setaffinity` / `taskset`).
* Separate threads strictly by priority (Market Data Ingestion > Risk/Strategy > Execution routing).


4. **Advanced Risk & Networking**
* Pre-calculate risk limits per symbol/session. Execute real-time VaR, Notional Value, and margin checks in `< 1µs`.
* Utilize Boost.Asio backed by `io_uring` (Linux) for ultimate networking performance.


5. **Hardware & OS Optimizations (Production)**
* Deploy on AlmaLinux / Debian Server with a low-latency kernel.
* Disable CPU Turbo Boost to prevent frequency scaling latency (jitter). Set CPU governor to `performance`.
* Enable Huge Pages to prevent TLB cache misses.



---

## 🚀 Execution Strategy: Horizontal Scaling

Launch multiple independent instances of the compiled binary, locked to separate CPU cores, driven dynamically by JSON configurations.

```bash
# Terminal 1: Run Binance BTC Bot pinned to Physical Core 4
taskset -c 4 ./build/bluestone --mode fix --config .env/binance_btc.json --instance 1

# Terminal 2: Run Exness FIX Bot pinned to Physical Core 5
taskset -c 5 ./build/bluestone --mode websocket --config .env/coinbase_fix.json --instance 2
```

``` Resource
# https://en.wikipedia.org/wiki/List_of_electronic_trading_platforms
```

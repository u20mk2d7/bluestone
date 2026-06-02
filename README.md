# 🏛️ Project bluestone: High-Frequency Trading Architecture Blueprint

**Objective:** To engineer a low-latency, institutional-grade Quantitative Trading Infrastructure that bridges traditional finance (TradFi) and cryptocurrency markets. The system is built on C++23, strictly adhering to zero-allocation hot paths, lock-free concurrency, and hardware-level thread affinity.
**Objective:** To engineer a low-latency, institutional-grade Quantitative Trading Infrastructure bridging TradFi (IBKR, FIX) and cryptocurrency markets (Binance, OKX, Coinbase).
## ⚠️ AI ASSISTANT DIRECTIVES: STRICT CODING STANDARDS
Whenever generating C++ code for Project Bluestone, the AI must strictly adhere to the following:
* **Language:** Pure C++ latest.
* **Style & Linting:** Strict adherence to the LLVM coding standards, Google C++ Style Guide, and C++ Core Guidelines.
* **Architecture:** Uncle Bob's Clean Code principles, SOLID design, and appropriate Gang of Four (GoF) Design Patterns.
* **Performance Rule:** Zero heap allocations (`new`/`malloc`), zero mutex locks, and zero `std::string` copies in the Hot Path (Tier 1) after initial startup.


## ⏱️ The Latency Horizons (Execution Tiers)
### Tier 1: The Hot Path (1 - 100 Nanoseconds)
* **Purpose:** The Core Trading Engine. This is where market data is parsed, the Order Book is maintained, and the actual decision to "Buy" or "Sell" is executed.
* **Technology:** Pure C++23 running in main RAM and L1/L2 CPU Cache. Utilizes `simdjson` for zero-allocation JSON parsing and `absl::flat_hash_map` for ultra-fast Order Book lookups.
* **Hardware Execution:** Runs on a dedicated `EventLoopWorker` thread permanently pinned to an isolated physical CPU core. It spins at 100% CPU capacity (busy-wait) and uses **zero** mutex locks or heap allocations during runtime.

### Tier 2: The Warm Path (100 - 200 Microseconds)
* **Purpose:** Shared State & Global Risk Management. This layer allows multiple independent bots (Binance, OKX, IBKR) to communicate, share global position limits, and calculate cross-exchange arbitrage.
* **Technology:** **Valkey** (KeyDB) running completely asynchronously via Unix Domain Sockets (UDS) or optimized TCP.

### Tier 3: The Cold Path (20 - 50 Milliseconds)
* **Purpose:** The Historian. This tier is purely for auditing, calculating PnL, tax records, and saving a permanent receipt of all events.
* **Technology:** **PostgreSQL** database fronted by a **PostgREST** proxy.
* **The Bridge:** Tier 1 never waits for Tier 3. Tier 1 sends data to Tier 3 using a lock-free mailbox (`boost::lockfree::spsc_queue`). A background thread (`PostgRESTLogger`) reads this mailbox and sends HTTP POST requests, politely yielding/sleeping when the mailbox is empty to save CPU cycles.

### Tier 4: The Management Plane (Seconds)
* **Purpose:** Web Dashboards, Python quantitative monitoring scripts, and User Interfaces.
* **The Golden Rule:** Tier 4 applications are **strictly banned** from communicating with the C++ trading engine (Tiers 1 & 2). They may only query the PostgreSQL database in Tier 3 to prevent any latency spikes in the core engine.

---

## 🌍 Infrastructure & Deployment Topology

### Core Dependencies
1. **QuickFIX / QuantLib:** Built from source and vendored locally for absolute hermetic builds.
2. **TWSAPI:** download source and save to root/third_party folder then use cmake link to library, protobuf build
2. **PostgreSQL:** For persistent relational state. ()
3. **Valkey:** For in-memory, distributed state sharing.
4. **ZeroMQ / WebSockets:** For high-throughput internal routing and exchange connections.
5. **PostgreSQL:** Abseil, Boost, simdjson, Protobuf, OpenSSL

### Deployment Flow
```text
┌──────────────────────────┐               ┌──────────────────────────┐
│   MacBook (Local Dev)    │   ZeroMQ /    │  Linux Server (Prod)     │
│  - Write C++23           │   WebSockets  │  - Low-Latency Kernel    │
│  - Test & Backtest       ├──────────────►│  - Direct Exchange I/O   │
│  - Compile Code Locally  │               │  - Runs 24/7 Untouched   │
└──────────────────────────┘               └──────────────────────────┘

TradingBotFramework/
├── CMakeLists.txt                 # Master Build File
├── .env/
│   └── *.json                     # Configs (loaded once at startup)
├── scripts/
│   └── deploy.sh                  # CI/CD and deployment scripts
├── build/                         # Release build (LTO, march=native)
└── src/
    ├── CMakeLists.txt
    ├── pch.hpp                    # Precompiled headers for fast compilation
    ├── common/
    │   ├── types.hpp              # Flat structs (No std::string in hot path)
    │   └── constants.hpp
    ├── utils/
    │   ├── lockfree/              # Boost.Lockfree + custom ring buffers
    │   ├── memory/                # Custom memory pool & arena allocators
    │   └── timing/                # High-precision timers (rdtsc, chrono)
    ├── core/
    │   ├── exchange/              # Abstract interfaces (minimal virtual calls)
    │   ├── fix/                   # Optimized QuickFIX wrapper (custom fast parser)
    │   ├── tws/                   # TWSAPI low-latency wrapper
    │   ├── strategy/              # Lock-free strategy engine
    │   ├── risk/                  # Ultra-fast risk engine (pre-check + lock-free)
    │   ├── marketdata/            # Lock-free order book + tick processor
    │   └── execution/             # Smart order router + direct execution
    ├── gateways/
    │   ├── binance/               # Thin & optimized exchange adapters
    │   ├── okx/
    │   ├── coinbase/
    │   └── interactivebrokers/
    └── bots/
        └── main.cpp               # Dynamic Entry point with core pinning

src/
├── core/
│   ├── exchange/              # Abstract interfaces
│   │   ├── i_exchange_connector.hpp
│   │   └── i_fix_session.hpp
│   ├── fix/                   # FIX Protocol Core
│   │   ├── fix_session.hpp
│   │   ├── fix_session.cpp
│   │   ├── quickfix_wrapper.hpp   # Thin wrapper over QuickFIX
│   │   └── message_handler.hpp
│   ├── marketdata/
│   ├── risk/
│   ├── strategy/
│   └── trade_engine.hpp
├── gateways/
│   ├── binance/
│   │   ├── binance_connector.hpp/cpp        # WebSocket (current)
│   │   ├── binance_fix_connector.hpp/cpp    # NEW: FIX version
│   │   └── binance_api.hpp
│   ├── okx/
│   └── interactivebrokers/
├── utils/
└── main.cpp

⚡ Critical Techniques for Sub-Microsecond Performance
1. Minimize Allocations & Copying
Memory Pools: Use custom memory pool allocators. Zero new or malloc calls after initialization.

Flat Structures: Replace std::string in hot paths with std::string_view or fixed-size char arrays via arena allocators.

Zero-Copy Parsing: Parse FIX and JSON messages strictly in place.

2. Lock-Free Everything
Queues: Utilize boost::lockfree::spsc_queue or MoodyCamel ConcurrentQueue for thread messaging.

Data Structures: Maintain a lock-free order book and pre-allocated risk checks.

3. Threading & CPU Affinity
Core Pinning: Pin each bot instance to specific physical CPU cores using OS-level affinity (sched_setaffinity / taskset).

Thread Segregation: Separate threads for:

Market Data Ingestion (Highest Priority)

Risk & Strategy Math

Execution / FIX Order Routing

4. Advanced Networking & Risk
Fast FIX: Avoid full QuickFIX processing in the hot path. Build a thin, fast FIX encoder/decoder on top of it.

Asynchronous I/O: Utilize Boost.Asio backed by io_uring (Linux) for ultimate networking performance.

Pre-Computed Risk: Pre-calculate risk limits per symbol/session. Execute real-time VaR, PnL, and margin checks using atomic variables in < 1µs. Circuit breakers trigger in nanoseconds.

5. Hardware & OS Optimizations (Production)
OS: AlmaLinux / Ubuntu Server with a low-latency kernel.

CPU Tuning: Disable Turbo Boost to prevent frequency scaling latency. Set CPU governor to performance.

Memory: Enable Huge Pages to prevent TLB misses.

Networking: Utilize Network Interface Cards (NICs) with hardware PTP timestamping.

🚀 Execution Strategy: Horizontal Scaling
Launch multiple independent instances of the compiled binary, locked to separate CPU cores, driven dynamically by JSON configurations.

***

### Next Steps, Architect.

Your documentation is locked in. Looking at your list of components at the bottom of your prompt, what do you want to build first?

1. **Ultra-fast Risk Engine** (lock-free, pre-checks)
2. **Optimized CMakeLists.txt** (with all release + LTO flags)
3. **Core Bot Skeleton** (main.cpp + thread pinning + config loader)
4. **Fast FIX Message Handler** (thin layer over QuickFIX)
5. **Lock-free Order Book + Market Data Processor**

My recommendation is to start with **#2 (Optimized CMakeLists.txt)** so we lock in the `-O3 -march=native -flto` compiler flags, followed immediately by **#3 (Core Bot Skeleton)** to get the `taskset` and CPU affinity logic actually running in C++. 


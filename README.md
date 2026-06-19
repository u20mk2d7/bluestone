# Project Bluestone: High-Frequency Trading Architecture Blueprint

**Objective:** To engineer a sub-microsecond, institutional-grade Quantitative Trading Infrastructure capable of cross-venue arbitrage. Project Bluestone bridges Traditional Finance (TradFi via LMAX/FIX) and Cryptocurrency markets (Binance via WebSockets/REST). The system is built on modern C++23, strictly adhering to zero-allocation hot paths, lock-free concurrency, and hardware-level thread affinity.

---

## ENGINEERING DIRECTIVES: STRICT CODING STANDARDS
All engineers (and AI Assistants) contributing to Project Bluestone must strictly adhere to the following constraints:
* **Language:** Pure C++23 or latest standard (Standard Template Library only in the core).
* **Style & Linting:** Strict adherence to LLVM coding standards, the Google C++ Style Guide, and C++ Core Guidelines.
* **Architecture:** Uncle Bob's Clean Code principles, SOLID design, and appropriate Gang of Four (GoF) Design Patterns.
* **The Golden Performance Rule:** **Zero** heap allocations (`new`/`malloc`), **zero** mutex locks, and **zero** `std::string` copies in the Hot Path (Tier 1) after the initial boot sequence.

---

## The Latency Horizons (Execution Tiers)

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

## Infrastructure & Dependencies

Project Bluestone utilizes a hermetically sealed Conan v2 build system to prevent ABI mismatches and ensure portability.

1. **QuickFIX & OpenSSL:** For secure, encrypted FIX protocol connections (Coinbase/TradFi).
2. **TWSAPI & Protobuf:** Vendored locally and built from source for Interactive Brokers.
3. **Abseil & Boost:** For flat hash maps, CLI parsing, and lock-free queues.
4. **simdjson:** For zero-allocation WebSocket JSON parsing.
5. **Valkey & PostgreSQL:** For external state management.

---

## SYSTEM TOPOLOGY: THE HYBRID BRIDGE
Project Bluestone normalizes fragmented market structures into a single, unified algorithmic pipeline.
```
┌────────────────────────┐             ┌────────────────────────┐
│   BINANCE (Crypto)     │             │    LMAX (TradFi)       │
│ wss://stream.binance   │             │ TargetCompID: LMXBDM   │
└───────────┬────────────┘             └───────────┬────────────┘
            │ JSON / WebSockets                    │ FIX Protocol (35=W)
            ▼                                      ▼
┌────────────────────────┐             ┌────────────────────────┐
│  Binance Gateway       │             │  QuickFIX Gateway      │
│  (Boost.Beast / SSL)   │             │  (Session Management)  │
└───────────┬────────────┘             └───────────┬────────────┘
            │ simdjson Parsing                     │ FIX Tag Extraction
            ▼                                      ▼
┌───────────────────────────────────────────────────────────────┐
│                    SHARED MEMORY BUS                          │
│               Lock-Free SPSC Ring Buffers                     │
└──────────────────────────────┬────────────────────────────────┘
                               │ NormalizedTicks
                               ▼
┌───────────────────────────────────────────────────────────────┐
│                    CORE HOT PATH (C++23)                      │
│                                                               │
│  ┌────────────────┐  ┌────────────────┐  ┌─────────────────┐  │
│  │ Book Builder   ├──► Strategy Engine├──► Risk Engine     │  │
│  └────────────────┘  └────────────────┘  └───────┬─────────┘  │
│                                                  │            │
│  ┌────────────────┐  ┌────────────────┐          │            │
│  │ Fast FIX Maker │◄─┤ Order Manager  │◄─────────┘            │
│  │ (Zero Alloc)   │  │ (OMS)          │                       │
│  └───────┬────────┘  └────────────────┘                       │
└──────────│────────────────────────────────────────────────────┘
           │ Raw char[] byte arrays
           ▼
┌────────────────────────┐
│  Execution Gateway     │
│  (QuickFIX Network I/O)│
└───────────┬────────────┘
            │
            ▼
    Exchange Matching
```

## Monorepo Directory Structure (The Unified Tree)

```text
bluestone/
├── CMakeLists.txt                 # Master Build (LTO, -march=native, -O3, -fno-exceptions for core)
├── .env/                          # Configs loaded strictly at boot
├── build/                         # Release binaries
└── src/
    ├── common/                    # Flat structs (POD types, zero std::string)
    ├── utils/                     # System-level utils (Config, CLI, Thread affinity)
    ├── core/                      # THE PURE LAYER (C++23 Standard Library ONLY)
    |   |-- ipc/                   # Inter-Process Communication
    │   ├── utils/                 # SPSC queues, memory arenas, TSC timers
    │   ├── fix/                   # Zero-allocation FIX encoders/decoders (Raw C++)
    │   ├── strategy/              # Lock-free alpha engines
    │   ├── risk/                  # O(1) Pre-trade limit checks & Kill switches
    │   ├── marketdata/            # Contiguous memory Order Book implementation
    │   └── oms/                   # Order State Machines
    ├── gateway/                   # THE DIRTY LAYER (Network I/O, Third-party libs)
    │   ├── binance/               # Boost.Beast WebSocket implementations
    │   ├── lmax/                  # QuickFIX Engine instances
    │   └── interface/             # Abstract base classes (IExchangeConnector)
    └── main.cpp                   # Wiring, Dependency Injection, and Ignition
```

---

## Critical Techniques for Sub-Microsecond Performance

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
* Deploy on AlmaLinux Server with a low-latency kernel.
* Disable CPU Turbo Boost to prevent frequency scaling latency (jitter). Set CPU governor to `performance`.
* Enable Huge Pages to prevent TLB cache misses.

4. Hardware/Software Symbiosis (Thread Topology)
We treat the Linux scheduler as a hindrance. The execution topology relies on strict core isolation using OS-level affinity (taskset, sched_setaffinity).

CPU 0 -> OS / Kernel Tasks / Interrupts

CPU 1 -> Binance Feed Handler (WebSockets ingress)

CPU 2 -> LMAX Feed Handler (QuickFIX Market Data ingress)

CPU 3 -> Order Book Builder (Normalizer)

CPU 4 -> Strategy Engine (Alpha Generation & Pricing)

CPU 5 -> Risk Engine & Order Management System (OMS)

CPU 6 -> Execution Gateway (QuickFIX Outbound egress)

CPU 7 -> Asynchronous Logger (spdlog) & PostgreSQL Writer

6. **LATENCY BUDGET & DATA FLOW**
```
Inbound Market Data Path (Tick to Signal)

Exchange NIC           -> Gateway        0.8 µs
Gateway Normalization  -> Ring Buffer    1.0 µs
Book Builder Update    -> Strategy       1.0 µs
-----------------------------------------------
Total Target                             < 3.0 µs
```

```
Outbound Order Path (Signal to Wire)
Strategy Signal        -> Risk Engine    0.5 µs
Risk Engine (Margin)   -> OMS            0.8 µs
OMS Routing            -> Core FIX Maker 0.5 µs
Core FIX (byte packing)-> Socket Flush   1.2 µs
-----------------------------------------------
Total Target                             < 3.0 µs
```
---

## 🚀 Execution Strategy: Horizontal Scaling
## 6. Production Infrastructure Mandates
```
To achieve sub-microsecond determinism, software alone is insufficient. The production environment must be rigorously tuned.

Kernel Bypass Networking: Utilize DPDK or Solarflare OpenOnload. The application must read packets directly from the NIC ring buffers, bypassing the Linux TCP/IP stack.

CPU Governor & C-States: Disable CPU Turbo Boost to eliminate frequency scaling latency (jitter). Enforce performance governor. Disable deep C-states in BIOS.

Memory TLB: Enable Linux Huge Pages (1GB size) to drastically reduce Translation Lookaside Buffer (TLB) misses.

Execution Strategy (Horizontal Scaling): Avoid vertical multi-threading within a single strategy. Launch independent, parameter-driven binary instances pinned to discrete CPU cores via JSON configs.

Launch multiple independent instances of the compiled binary, locked to separate CPU cores, driven dynamically by JSON configurations.
```

```bash
# Terminal 1: Run bluestone pinned to Physical Core 4 and bluestone has instance 1
taskset -c 4 ./build/bluestone --exchange binance --config .env/binance.cfg --instance 1
# Terminal 2: Run bluestone pinned to Physical Core 5 and bluestone has instance 2
taskset -c 5 ./build/bluestone --exchange lmax --config .env/lmax.cfg --instance 2
```

``` Resource
# https://en.wikipedia.org/wiki/List_of_electronic_trading_platforms
# https://en.wikipedia.org/wiki/Financial_Information_eXchange
# https://en.wikipedia.org/wiki/FIXatdl

```
[ Tier 1 Banks ] 
(JPM, Citi, Goldman Sachs, Deutsche Bank)
       │
       ▼  (Broadcast raw pricing feeds)
[ Tier 1 Venues / Prime Brokers ] 
(LMAX Exchange, Interactive Brokers, Finalto)
       │
       ▼  (Provide API / Retail / Pro Access)
[ Individual / Corporate Pro Accounts ]



https://www.lmax.com/global/demo-login
https://docs.lmax.com/broker-market-data-api/

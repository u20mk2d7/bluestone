# QuickFIX/C++ Trading Client Architecture

library:
spdlog
PostgreSQL
Docker

## Overview
This system connects to a broker or exchange using the FIX protocol through QuickFIX/C++.

Responsibilities:

* Manage FIX sessions
* Send trading orders
* Receive execution reports
* Maintain order state
* Persist messages and logs
* Provide APIs for upstream applications

---

# High-Level Architecture

```text

┌───────────────────────┐
│ Trading Application   │
│ (Strategy/UI/API)     │
└───────────┬───────────┘
            │
            ▼
┌───────────────────────┐
│ Order Service         │
│ - Create Orders       │
│ - Cancel Orders       │
│ - Replace Orders      │
└───────────┬───────────┘
            │
            ▼
┌───────────────────────┐
│ FIX Gateway           │
│ QuickFIX/C++          │
│                       │
│ - Session Mgmt        │
│ - Message Routing     │
│ - Sequence Numbers    │
│ - Heartbeats          │
└───────────┬───────────┘
            │
            ▼
┌───────────────────────┐
│ Broker / Exchange     │
└───────────────────────┘
```

---

# Component Diagram

```text
src/
├── app/
│   └── main.cpp
│
├── fix/
│   ├── FixApplication.h
│   ├── FixApplication.cpp
│   ├── SessionManager.h
│   └── SessionManager.cpp
│
├── services/
│   ├── OrderService.h
│   ├── OrderService.cpp
│   ├── ExecutionService.h
│   └── ExecutionService.cpp
│
├── models/
│   ├── Order.h
│   ├── Execution.h
│   └── Position.h
│
├── storage/
│   ├── OrderRepository.h
│   └── OrderRepository.cpp
│
├── config/
│   └── client.cfg
│
└── logs/
```

---

# Core Modules

## 1. FixApplication

Implements QuickFIX callbacks.

```cpp
class FixApplication :
    public FIX::Application,
    public FIX44::MessageCracker
{
};
```

Responsibilities:

* Handle logon/logout
* Receive FIX messages
* Dispatch messages
* Route execution reports

Callbacks:

```cpp
onCreate()
onLogon()
onLogout()
toAdmin()
fromAdmin()
toApp()
fromApp()
```

---

## 2. Session Manager

Responsible for FIX connectivity.

```text
SessionManager
 ├── Start()
 ├── Stop()
 ├── Reconnect()
 └── SendMessage()
```

Manages:

* SocketInitiator
* Session IDs
* Reconnect logic

---

## 3. Order Service

Business layer.

Responsibilities:

* Create New Order Single
* Create Cancel Request
* Create Replace Request

Example:

```cpp
submitOrder()
cancelOrder()
replaceOrder()
```

---

## 4. Execution Service

Processes incoming reports.

Receives:

```text
35=8 ExecutionReport
```

Updates:

* Order status
* Filled quantity
* Average price

---

## 5. Repository Layer

Stores business data.

Tables:

### Orders

```text
OrderID
ClOrdID
Symbol
Side
Qty
Price
Status
```

### Executions

```text
ExecID
OrderID
FillQty
FillPrice
Timestamp
```

---

# Message Flow

## Logon

```text
Client
  |
  | 35=A Logon
  |
Broker
```

QuickFIX establishes session.

---

## New Order

```text
Application
    |
    ▼
OrderService
    |
    ▼
FIX Gateway
    |
35=D NewOrderSingle
    |
Broker
```

---

## Execution Report

```text
Broker
    |
35=8 ExecutionReport
    |
FIX Gateway
    |
ExecutionService
    |
OrderRepository
```

---

# Order Lifecycle

```text
NEW
 │
 ▼
SENT
 │
 ▼
ACKNOWLEDGED
 │
 ├─────────────► REJECTED
 │
 ▼
PARTIALLY_FILLED
 │
 ▼
FILLED
 │
 ▼
CLOSED
```

Alternative path:

```text
ACKNOWLEDGED
      │
      ▼
CANCELED
```

---

# FIX Message Mapping

| FIX MsgType | Meaning               |
| ----------- | --------------------- |
| 35=A        | Logon                 |
| 35=5        | Logout                |
| 35=0        | Heartbeat             |
| 35=D        | New Order Single      |
| 35=F        | Order Cancel Request  |
| 35=G        | Order Replace Request |
| 35=8        | Execution Report      |
| 35=3        | Reject                |

---

# Configuration

```ini
[DEFAULT]

ConnectionType=initiator
HeartBtInt=30

FileStorePath=store
FileLogPath=logs

ReconnectInterval=30

[SESSION]

BeginString=FIX.4.4

SenderCompID=CLIENT1
TargetCompID=BROKER1

SocketConnectHost=127.0.0.1
SocketConnectPort=5001
```

---

# Runtime Sequence

```text
Application Start
        │
        ▼
Load Config
        │
        ▼
Create SessionManager
        │
        ▼
Start SocketInitiator
        │
        ▼
FIX Logon
        │
        ▼
Ready For Trading
        │
        ▼
Send Orders
        │
        ▼
Receive Executions
        │
        ▼
Update State
        │
        ▼
Shutdown
```

---

# Future Enhancements

* Market Data Support
* Risk Management
* Position Management
* Persistence Database
* REST API Layer
* WebSocket Streaming
* High Availability Failover
* Multiple FIX Sessions
* OMS/EMS Integration

---

# Technology Stack

| Layer         | Technology   |
| ------------- | ------------ |
| FIX Engine    | QuickFIX/C++ |
| Language      | C++23+       |
| Logging       | spdlog       |
| Storage       | PostgreSQL   |
| Build         | CMake        |
| Testing       | GoogleTest   |
| Serialization | simdJSOn     |
| Monitoring    | Prometheus   |
| Deployment    | Docker       |

```
```
QuickFIX session layer
MessageCracker handlers
Order Manager (OMS)
Market Data Gateway
Risk Engine
PostgreSQL persistence
Kafka event bus
Docker deployment
# HFT Trading Platform Architecture (2026)

## Design Goals

* Sub-microsecond internal processing
* Lock-free message passing
* NUMA-aware deployment
* Kernel bypass networking
* Deterministic latency
* Exchange colocation
* High availability

---

# System Architecture

```text
┌──────────────────────────────────────────────────────┐
│ Exchange                                             │
└──────────────┬───────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────┐
│ Network Layer                                        │
│                                                      │
│ Solarflare / ExaNIC / Mellanox NIC                  │
│ DPDK / AF_XDP / Onload                              │
└──────────────┬───────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────┐
│ Market Data Gateway                                  │
│                                                      │
│ ITCH / OUCh / SBE / Binary Feeds                    │
│ Packet Decoder                                       │
│ Gap Detection                                        │
│ Feed Arbitration                                     │
└──────────────┬───────────────────────────────────────┘
               │
               ▼
┌──────────────────────────────────────────────────────┐
│ Shared Memory Bus                                    │
│                                                      │
│ Lock-Free Ring Buffer                               │
│ NUMA Local                                           │
└──────┬───────────────────────┬───────────────────────┘
       │                       │
       ▼                       ▼

┌──────────────┐       ┌─────────────────┐
│ Risk Engine  │       │ Strategy Engine │
└──────┬───────┘       └────────┬────────┘
       │                        │
       └──────────┬─────────────┘
                  │
                  ▼

┌──────────────────────────────────────────────────────┐
│ Order Manager (OMS)                                  │
└──────────────┬───────────────────────────────────────┘
               │
               ▼

┌──────────────────────────────────────────────────────┐
│ Execution Gateway                                    │
│                                                      │
│ FIX / OUCh / Native Exchange API                    │
└──────────────┬───────────────────────────────────────┘
               │
               ▼
          Exchange
```

---

# Service Layout

```text
services/

├── md_gateway
├── feed_handler
├── order_gateway
├── strategy_engine
├── risk_engine
├── position_engine
├── market_maker
├── arbitrage_engine
├── persistence
├── metrics
└── replay_engine
```

---

# Where QuickFIX Lives

```text
Market Data
      │
      ▼
Strategy
      │
      ▼
Order Manager
      │
      ▼
QuickFIX Gateway
      │
      ▼
Broker / Exchange
```

QuickFIX is isolated inside the Execution Gateway.

The strategy engine never directly touches FIX tags.

---

# Market Data Path

```text
NIC
 │
 ▼
Kernel Bypass
 │
 ▼
Feed Handler
 │
 ▼
Order Book Builder
 │
 ▼
Strategy
```

Target:

```text
Feed -> Strategy

< 5 microseconds
```

---

# Order Path

```text
Signal
  │
  ▼
Risk Check
  │
  ▼
OMS
  │
  ▼
Execution Gateway
  │
  ▼
Exchange
```

Target:

```text
Signal -> Wire

< 10 microseconds
```

---

# Thread Model

CPU Core 0

```text
Market Data Feed A
```

CPU Core 1

```text
Market Data Feed B
```

CPU Core 2

```text
Order Book Builder
```

CPU Core 3

```text
Strategy
```

CPU Core 4

```text
Risk Engine
```

CPU Core 5

```text
Order Gateway
```

CPU Core 6

```text
QuickFIX Session
```

CPU Core 7

```text
Logging / Monitoring
```

---

# Memory Layout

Avoid:

```cpp
std::mutex
std::shared_ptr
dynamic allocation
```

Prefer:

```cpp
boost::lockfree
folly::MPMCQueue
std::pmr
fixed-size pools
```

---

# Market Data Gateway

Responsibilities:

* Decode binary feeds
* Sequence tracking
* Gap recovery
* Snapshot recovery
* Order book construction

Produces:

```cpp
struct MarketUpdate
{
    uint64_t ts;
    uint32_t symbol;
    double bid;
    double ask;
};
```

---

# Strategy Engine

Consumes:

```cpp
MarketUpdate
```

Produces:

```cpp
TradeSignal
```

Example:

```cpp
BUY
SELL
CANCEL
REPLACE
```

---

# Risk Engine

Checks:

* Max position
* Max order size
* Max loss
* Fat-finger protection
* Kill switch

Must complete in:

```text
< 1 microsecond
```

---

# OMS

Tracks:

```cpp
Order
Execution
Position
PnL
```

Maintains exchange state consistency.

---

# Persistence Layer

Asynchronous only.

Never block trading threads.

```text
Trading Thread
     │
     ▼
Ring Buffer
     │
     ▼
Logger Thread
     │
     ▼
PostgreSQL
```

---

# Monitoring

Metrics:

```text
Latency
Orders/sec
Market Messages/sec
Fill Ratio
PnL
Position
```

Export:

```text
Prometheus
Grafana
```

---

# Deployment

```text
Colocation Rack
        │
        ▼
Dual Xeon / EPYC
        │
        ▼
25G / 100G NIC
        │
        ▼
Linux RT Kernel
```

---

# Technology Stack (2026)

| Layer         | Technology             |
| ------------- | ---------------------- |
| Language      | C++23                  |
| FIX Engine    | QuickFIX 1.16          |
| Build         | CMake                  |
| Logging       | spdlog                 |
| Queue         | folly / boost lockfree |
| Storage       | PostgreSQL             |
| Monitoring    | Prometheus             |
| Dashboard     | Grafana                |
| Network       | DPDK / AF_XDP          |
| Serialization | SBE                    |
| Deployment    | Docker / Kubernetes    |

```
```
actual C++23+ code structure (thread pinning, lock-free queues, market-data decoder, OMS, and QuickFIX gateway classes
# HFT Trading System - C++23 Architecture

## Source Tree

```text
hft-platform/

├── apps/
│   ├── trading_engine/
│   ├── market_gateway/
│   └── replay_engine/
│
├── core/
│   ├── cpu/
│   │   ├── cpu_affinity.hpp
│   │   ├── numa.hpp
│   │   └── thread_factory.hpp
│   │
│   ├── memory/
│   │   ├── object_pool.hpp
│   │   ├── arena.hpp
│   │   └── cacheline.hpp
│   │
│   ├── queue/
│   │   ├── spsc_ring.hpp
│   │   ├── mpsc_ring.hpp
│   │   └── sequence.hpp
│   │
│   ├── time/
│   │   ├── rdtsc.hpp
│   │   └── timestamp.hpp
│   │
│   └── logging/
│
├── marketdata/
│   ├── decoder/
│   │   ├── itch_decoder.hpp
│   │   ├── ouch_decoder.hpp
│   │   └── sbe_decoder.hpp
│   │
│   ├── book/
│   │   ├── order_book.hpp
│   │   ├── level.hpp
│   │   └── book_builder.hpp
│   │
│   └── feed_handler.hpp
│
├── strategy/
│   ├── market_maker/
│   ├── arbitrage/
│   └── signal_engine.hpp
│
├── risk/
│   ├── risk_engine.hpp
│   ├── limits.hpp
│   └── kill_switch.hpp
│
├── oms/
│   ├── order_manager.hpp
│   ├── execution_manager.hpp
│   ├── position_manager.hpp
│   └── pnl_manager.hpp
│
├── gateway/
│   ├── fix/
│   │   ├── fix_application.hpp
│   │   ├── fix_session.hpp
│   │   ├── quickfix_gateway.hpp
│   │   └── fix_message_factory.hpp
│   │
│   └── native/
│
├── persistence/
│   ├── writer.hpp
│   └── postgres_sink.hpp
│
└── tests/
```

---

# Runtime Topology

```text
Core 0
┌─────────────────┐
│ Feed Handler A  │
└────────┬────────┘
         │
         ▼

Core 1
┌─────────────────┐
│ Feed Handler B  │
└────────┬────────┘
         │
         ▼

Core 2
┌─────────────────┐
│ Book Builder    │
└────────┬────────┘
         │
         ▼

Core 3
┌─────────────────┐
│ Strategy Engine │
└────────┬────────┘
         │
         ▼

Core 4
┌─────────────────┐
│ Risk Engine     │
└────────┬────────┘
         │
         ▼

Core 5
┌─────────────────┐
│ OMS             │
└────────┬────────┘
         │
         ▼

Core 6
┌─────────────────┐
│ QuickFIX GW     │
└────────┬────────┘
         │
         ▼
       Exchange

Core 7
┌─────────────────┐
│ Logger          │
└─────────────────┘
```

---

# Thread Pinning

```cpp
class CpuAffinity
{
public:

    static void pin(
        std::thread& thread,
        int cpu_id);
};
```

Usage:

```cpp
std::thread strategy_thread(
    [&] { strategy.run(); });

CpuAffinity::pin(
    strategy_thread,
    3);
```

Mapping:

```text
CPU0 -> Feed A
CPU1 -> Feed B
CPU2 -> Book
CPU3 -> Strategy
CPU4 -> Risk
CPU5 -> OMS
CPU6 -> Gateway
CPU7 -> Logging
```

---

# Lock-Free Pipeline

```text
Feed
  │
  ▼
SPSC Queue
  │
  ▼
Book Builder
  │
  ▼
SPSC Queue
  │
  ▼
Strategy
  │
  ▼
SPSC Queue
  │
  ▼
Risk
  │
  ▼
SPSC Queue
  │
  ▼
OMS
```

Every stage owns exactly one thread.

No mutexes.

---

# SPSC Ring Buffer

```cpp
template<typename T, size_t N>
class SPSCRing
{
public:

    bool push(const T& value);

    bool pop(T& value);

private:

    alignas(64)
    std::atomic<size_t> head_;

    alignas(64)
    std::atomic<size_t> tail_;

    std::array<T, N> buffer_;
};
```

Characteristics:

```text
Producer = 1
Consumer = 1

Lock Free
Cache Friendly
Deterministic
```

---

# Market Data Flow

```text
NIC
 │
 ▼
ITCH Decoder
 │
 ▼
MarketUpdate
 │
 ▼
Book Builder
 │
 ▼
Order Book
 │
 ▼
Strategy
```

---

# Market Update Event

```cpp
struct MarketUpdate
{
    uint64_t timestamp;

    uint32_t instrument;

    double bid;

    double ask;

    uint32_t bid_qty;

    uint32_t ask_qty;
};
```

---

# Decoder Layer

```cpp
class ItchDecoder
{
public:

    bool decode(
        std::span<const uint8_t> packet,
        MarketUpdate& out);
};
```

Responsibilities:

```text
Sequence Validation
Packet Parsing
Gap Detection
Recovery Trigger
```

---

# Order Book

```cpp
class OrderBook
{
public:

    void add_order(...);

    void cancel_order(...);

    void execute_order(...);

    BestBidAsk best_bid_ask() const;
};
```

---

# Strategy Engine

```cpp
class StrategyEngine
{
public:

    void on_market_update(
        const MarketUpdate& update);

private:

    Signal generate_signal(...);
};
```

Produces:

```cpp
enum class SignalType
{
    BUY,
    SELL,
    CANCEL,
    REPLACE
};
```

---

# Risk Engine

```cpp
class RiskEngine
{
public:

    bool validate(
        const OrderRequest& req);
};
```

Checks:

```text
Position Limit
Loss Limit
Order Size Limit
Symbol Restrictions
Kill Switch
```

---

# OMS

```cpp
class OrderManager
{
public:

    OrderId submit(
        const OrderRequest& request);

    void on_execution(
        const ExecutionReport&);
};
```

Tracks:

```text
Pending
Live
Partial
Filled
Canceled
Rejected
```

---

# OMS State Diagram

```text
           New
            │
            ▼
         Pending
            │
            ▼
          Live
      ┌─────┴─────┐
      ▼           ▼

 Partial      Cancelled
      │
      ▼

   Filled
```

---

# Gateway Layer

```text
OMS
 │
 ▼
Gateway
 │
 ▼
QuickFIX
 │
 ▼
Broker
```

---

# QuickFIX Components

```text
QuickFixGateway
        │
        ├── FixApplication
        │
        ├── SessionManager
        │
        └── MessageFactory
```

---

# QuickFixGateway

```cpp
class QuickFixGateway
{
public:

    void start();

    void stop();

    bool send_order(
        const OrderRequest& request);

private:

    FIX::SessionID session_;
};
```

---

# FIX Application

```cpp
class FixApplication :
    public FIX::Application,
    public FIX44::MessageCracker
{
public:

    void onLogon(
        const FIX::SessionID&) override;

    void onLogout(
        const FIX::SessionID&) override;

    void onMessage(
        const FIX44::ExecutionReport&,
        const FIX::SessionID&);
};
```

---

# Outbound Order Flow

```text
Strategy
   │
   ▼
Risk
   │
   ▼
OMS
   │
   ▼
Gateway
   │
   ▼
FIX44::NewOrderSingle
   │
   ▼
QuickFIX
   │
   ▼
Exchange
```

---

# Inbound Execution Flow

```text
Exchange
   │
   ▼
QuickFIX
   │
   ▼
ExecutionReport
   │
   ▼
Gateway
   │
   ▼
OMS
   │
   ▼
Position Manager
```

---

# Position Manager

```cpp
class PositionManager
{
public:

    void on_fill(
        const Fill& fill);

    int64_t position(
        InstrumentId id) const;
};
```

---

# Async Persistence

Never write from trading threads.

```text
Trading Thread
        │
        ▼
Lock-Free Queue
        │
        ▼
Persistence Thread
        │
        ▼
PostgreSQL
```

---

# Latency Budget

```text
Feed Decode      0.8 us
Book Update      1.0 us
Strategy         1.5 us
Risk             0.5 us
OMS              0.8 us
Gateway          2.0 us
-----------------------
Total            6.6 us
```

---

# Future Expansion

```text
Multi Exchange
      │
      ├── CME
      ├── NASDAQ
      ├── NYSE
      ├── Binance
      └── Coinbase
      |-- LMAX
      |-- IBKR

Cross Venue Arbitrage

Smart Order Router

FPGA Feed Handler

Kernel Bypass Networking

Hardware Timestamping
```



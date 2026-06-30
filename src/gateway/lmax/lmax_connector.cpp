#include "lmax_connector.hpp"

#include <quickfix/Application.h>
#include <quickfix/FixValues.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Session.h>
#include <quickfix/fix44/ExecutionReport.h>
#include <quickfix/fix44/MarketDataRequest.h>
#include <quickfix/fix44/MarketDataSnapshotFullRefresh.h>

#include <iostream>
#include <string>

namespace bluestone {
  LMAXConnector::LMAXConnector(const bluestone::utils::ExchangeConfig& cfg,
                               std::shared_ptr<bluestone::TradeQueue> queue)
      : cfg_(cfg), queue_(std::move(queue)) {
    //
    settings_ = std::make_unique<FIX::SessionSettings>(cfg_.exchange_cfg);
    store_factory_ = std::make_unique<FIX::FileStoreFactory>(*settings_);
    log_factory_ = std::make_unique<FIX::FileLogFactory>(*settings_);

    initiator_ = std::make_unique<FIX::ThreadedSocketInitiator>(
        *this, *store_factory_, *settings_, *log_factory_);
  }

  LMAXConnector::~LMAXConnector() { this->disconnect(); }

  void LMAXConnector::connect() {
    std::cout << "[LMAX] Igniting QuickFIX Initiator...\n";
    initiator_->start();
  }

  void LMAXConnector::disconnect() {
    if (initiator_) {
      std::cout << "[LMAX] Stopping Initiator...\n";
      initiator_->stop();
    }
  }

  void LMAXConnector::reconnect() {
    std::cout << "[LMAX] Reconnecting...\n";
    disconnect();
    connect();
  }

  // Implement the interface method using your FIX logic
  void LMAXConnector::subscribe_market_data(int req_id,
                                            const std::string& symbol) {
    FIX44::MarketDataRequest mdRequest;

    // 1. MDReqID (Tag 262): Use the universal req_id passed from the Core
    // Engine
    mdRequest.set(FIX::MDReqID("REQ_" + std::to_string(req_id)));

    // 2. SubscriptionRequestType (Tag 263): 1 = Snapshot + Updates (Subscribe)
    mdRequest.set(FIX::SubscriptionRequestType(
        FIX::SubscriptionRequestType_SNAPSHOT_AND_UPDATES));

    // 3. MarketDepth (Tag 264): 1 = Top of Book (Best Bid/Offer), 0 = Full Book
    mdRequest.set(FIX::MarketDepth(1));

    // 4. MDUpdateType (Tag 265): 0 = Full Refresh
    mdRequest.set(FIX::MDUpdateType(FIX::MDUpdateType_FULL_REFRESH));

    // 5. NoMDEntryTypes (Tag 267): Specify we want both Bids (0) and Offers (1)
    FIX44::MarketDataRequest::NoMDEntryTypes entryTypesGroup;
    entryTypesGroup.set(FIX::MDEntryType(FIX::MDEntryType_BID));
    mdRequest.addGroup(entryTypesGroup);

    entryTypesGroup.set(FIX::MDEntryType(FIX::MDEntryType_OFFER));
    mdRequest.addGroup(entryTypesGroup);

    // 6. NoRelatedSym (Tag 146): Use the universal symbol passed from the Core
    // Engine
    FIX44::MarketDataRequest::NoRelatedSym symbolGroup;
    symbolGroup.set(FIX::SecurityID(symbol));  // e.g., "4001" for EUR/USD
    symbolGroup.set(
        FIX::SecurityIDSource(FIX::SecurityIDSource_EXCHANGE_SYMBOL));
    mdRequest.addGroup(symbolGroup);

    // 7. Send the request explicitly to your Market Data session
    FIX::Session::sendToTarget(mdRequest, FIX::SenderCompID(cfg_.user_id),
                               FIX::TargetCompID("LMXBDM"));

    std::cout << "[LMAX] Sent Market Data Request (ID: " << req_id
              << ") for Instrument: " << symbol << '\n';
  }

  void LMAXConnector::onCreate(const FIX::SessionID& sessionID) {
    std::cout << "Session Created: " << sessionID << '\n';
  }

  void LMAXConnector::onLogon(const FIX::SessionID& sessionID) {
    if (sessionID.getTargetCompID() == "LMXBDMD") {
      std::cout << "Market Data session connected!" << '\n';
    } else if (sessionID.getTargetCompID() == "LMXBDD") {
      std::cout << "Trading session connected!" << '\n';
    }
  }

  void LMAXConnector::onLogout(const FIX::SessionID& sessionID) {
    std::cout << "Logout - Session: " << sessionID << '\n';
  }

  void LMAXConnector::toAdmin(FIX::Message& message,
                              const FIX::SessionID& sessionID) {
    FIX::MsgType msgType;
    message.getHeader().getField(msgType);

    if (msgType == FIX::MsgType_Logon) {
      message.setField(FIX::Username(cfg_.user_id));
      // LMAX requires Tag 554 for Password.
      // Note: Make sure to check if you need to use FIX::Password instead of
      // RawData
      message.setField(FIX::Password(cfg_.password));
      message.setField(FIX::ResetSeqNumFlag(true));
    }
  }

  void LMAXConnector::fromAdmin(const FIX::Message& message,
                                const FIX::SessionID& sessionID) {}

  void LMAXConnector::toApp(FIX::Message& message,
                            const FIX::SessionID& sessionID) {}

  void LMAXConnector::fromApp(const FIX::Message& message,
                              const FIX::SessionID& sessionID) {
    crack(message, sessionID);
  }

  // Handle incoming Execution Reports (35=8)
  void LMAXConnector::onMessage(const FIX44::ExecutionReport& message,
                                const FIX::SessionID& sessionID) {
    FIX::OrderID orderID;
    FIX::ExecType execType;

    // Safely extract fields if they exist
    if (message.isSetField(orderID)) message.get(orderID);
    if (message.isSetField(execType)) message.get(execType);

    std::cout << "\n[TRADING] Execution Report Received!" << '\n';
    std::cout << "Order ID: " << orderID.getValue()
              << " | ExecType: " << execType.getValue() << '\n';
  }

  // Handle incoming Market Data (35=W)
  void LMAXConnector::onMessage(
      const FIX44::MarketDataSnapshotFullRefresh& message,
      const FIX::SessionID& sessionID) {
    //
    bluestone::NormalizeTick tick{};
    tick.gateway_recv_tsc = bluestone::utils::TSCClock::now();
    // uint64_t core_recv_tsc = bluestone::utils::TSCClock::now(); // Example:
    // "Tick # 1,000,600"
    //
    FIX::Symbol symbol;
    if (message.isSetField(symbol)) {
      message.get(symbol);
      tick.instrument_id = 4001;
    }

    // 2. Extract the NoMDEntries field to get the total count
    FIX::NoMDEntries noMDEtries;
    if (message.isSetField(noMDEtries)) {
      message.get(noMDEtries);  // Get the count of repeating groups
      FIX44::MarketDataSnapshotFullRefresh::NoMDEntries group;
      FIX::MDEntryType entryType;
      FIX::MDEntryPx entryPx;
      FIX::MDEntrySize entrySz;

      for (int i = 0; i < noMDEtries.getValue(); ++i) {
        message.getGroup(i, group);
        group.get(entryType);
        group.get(entryPx);

        if (group.isSetField(entrySz)) {
          group.get(entrySz);
        } else {
          entrySz.setValue(0);
        }

        if (entryType == FIX::MDEntryType_BID) {
          tick.bid_price = entryPx.getValue();
          tick.bid_qty = entrySz.getValue();
        } else if (entryType == FIX::MDEntryType_OFFER) {
          tick.ask_price = entryPx.getValue();
          tick.ask_qty = entrySz.getValue();
        }
      }

      if (queue_) {
        queue_->push(tick);
      }
    }
  }

}  // namespace bluestone

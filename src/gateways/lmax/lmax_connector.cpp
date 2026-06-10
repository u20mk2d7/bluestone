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

#include "lmax_connector.hpp"

// Handle incoming Execution Reports (35=8)
void LMAXApplication::onMessage(const FIX44::ExecutionReport& message,
                                const FIX::SessionID& sessionID) {
  FIX::OrderID orderID;
  FIX::ExecType execType;

  // Safely extract fields if they exist
  if (message.isSetField(orderID)) message.get(orderID);
  if (message.isSetField(execType)) message.get(execType);

  std::cout << "\n[TRADING] Execution Report Received!" << std::endl;
  std::cout << "Order ID: " << orderID.getValue()
            << " | ExecType: " << execType.getValue() << std::endl;
}

// Handle incoming Market Data (35=W)
void LMAXApplication::onMessage(
    const FIX44::MarketDataSnapshotFullRefresh& message,
    const FIX::SessionID& sessionID) {
  FIX::Symbol symbol;
  if (message.isSetField(symbol)) message.get(symbol);

  std::cout << "\n[MARKET DATA] Price Update for: " << symbol.getValue()
            << std::endl;

  // 1. Extract the NoMDEntries field to get the total count
  FIX::NoMDEntries noMDEntries;
  if (message.isSetField(noMDEntries)) {
    message.get(noMDEntries);  // Get the count of repeating groups

    FIX44::MarketDataSnapshotFullRefresh::NoMDEntries group;
    FIX::MDEntryType entryType;
    FIX::MDEntryPx entryPx;

    // 2. Loop using the extracted value
    for (int i = 1; i <= noMDEntries.getValue(); ++i) {
      message.getGroup(i, group);
      group.get(entryType);
      group.get(entryPx);

      if (entryType == FIX::MDEntryType_BID) {
        std::cout << "   BID: " << entryPx.getValue() << std::endl;
      } else if (entryType == FIX::MDEntryType_OFFER) {
        std::cout << "   ASK: " << entryPx.getValue() << std::endl;
      }
    }
  }
}

// Call this function AFTER your Market Data session successfully logs on
void LMAXApplication::requestMarketData(const std::string& instrumentId) {
  FIX44::MarketDataRequest mdRequest;

  // 1. MDReqID (Tag 262): A unique identifier you create for this request
  mdRequest.set(FIX::MDReqID("REQ_" + instrumentId));

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

  // 6. NoRelatedSym (Tag 146): Specify the instrument we want
  FIX44::MarketDataRequest::NoRelatedSym symbolGroup;
  symbolGroup.set(FIX::SecurityID(
      instrumentId));  // LMAX uses SecurityID, e.g., "4001" for EUR/USD
  symbolGroup.set(FIX::SecurityIDSource(
      FIX::SecurityIDSource_EXCHANGE_SYMBOL));  // Tag 22=8
  mdRequest.addGroup(symbolGroup);

  // 7. Send the request explicitly to your Market Data session
  FIX::Session::sendToTarget(mdRequest, FIX::SenderCompID("DuyHaDemo"),
                             FIX::TargetCompID("LMXBDM"));

  std::cout << "Sent Market Data Request for Instrument: " << instrumentId
            << std::endl;
}

// Implementation of the methods declared in lmax_connector.hpp
void LMAXApplication::onCreate(const FIX::SessionID& sessionID) {
  std::cout << "Session Created: " << sessionID << std::endl;
}

void LMAXApplication::onLogon(const FIX::SessionID& sessionID) {
  if (sessionID.getTargetCompID() == "LMXBDMD") {
    std::cout << "Market Data session connected!" << std::endl;
  } else if (sessionID.getTargetCompID() == "LMXBDD") {
    std::cout << "Trading session connected!" << std::endl;
  }
}

void LMAXApplication::onLogout(const FIX::SessionID& sessionID) {
  std::cout << "Logout - Session: " << sessionID << std::endl;
}

void LMAXApplication::toAdmin(FIX::Message& message,
                              const FIX::SessionID& sessionID) {
  FIX::MsgType msgType;
  message.getHeader().getField(msgType);

  if (msgType == FIX::MsgType_Logon) {
    message.setField(FIX::Username("DuyHaDemo"));
    // LMAX requires Tag 554 for Password.
    // Note: Make sure to check if you need to use FIX::Password instead of
    // RawData
    message.setField(FIX::Password(readconfig from file.env / app.cfg));
    message.setField(FIX::ResetSeqNumFlag(true));
  }
}

void LMAXApplication::fromAdmin(const FIX::Message& message,
                                const FIX::SessionID& sessionID) {}

void LMAXApplication::toApp(FIX::Message& message,
                            const FIX::SessionID& sessionID) {}

void LMAXApplication::fromApp(const FIX::Message& message,
                              const FIX::SessionID& sessionID) {
  crack(message, sessionID);
}

#pragma once
#ifndef LMAX_CONNECTOR_HPP
#define LMAX_CONNECTOR_HPP

#include <quickfix/Application.h>
#include <quickfix/FileLog.h>
#include <quickfix/FileStore.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/ThreadedSocketInitiator.h>
#include <quickfix/fix44/ExecutionReport.h>
#include <quickfix/fix44/MarketDataSnapshotFullRefresh.h>

#include <memory>
#include <string>

#include "core/exchange/i_exchange_connector.hpp"

namespace bluestone {
  class LMAXConnector : public bluestone::IExchangeConnector,
                        public FIX::Application,
                        public FIX::MessageCracker {
    //
   public:
    explicit LMAXConnector(const std::string& config_file);
    ~LMAXConnector() override;

    void onCreate(const FIX::SessionID& sessionID) override;
    void onLogon(const FIX::SessionID& sessionID) override;
    void onLogout(const FIX::SessionID& sessionID) override;
    void toAdmin(FIX::Message& message,
                 const FIX::SessionID& sessionID) override;
    void fromAdmin(const FIX::Message& message,
                   const FIX::SessionID& sessionID) override;
    void toApp(FIX::Message& message, const FIX::SessionID& sessionID) override;
    void fromApp(const FIX::Message& message,
                 const FIX::SessionID& sessionID) override;
    void requestMarketData(const std::string& instrumentId);

    // --- Add these MessageCracker callbacks ---
    void onMessage(const FIX44::ExecutionReport& message,
                   const FIX::SessionID& sessionID) override;
    void onMessage(const FIX44::MarketDataSnapshotFullRefresh& message,
                   const FIX::SessionID& sessionID) override;
  };
}  // namespace bluestone
   //
#endif

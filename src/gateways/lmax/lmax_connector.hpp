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
#include "utils/config_loader.hpp"

namespace bluestone {
  class LMAXConnector : public bluestone::IExchangeConnector,
                        public FIX::Application,
                        public FIX::MessageCracker {
    //
   private:
    // --- Encapsulated QuickFIX State ---
    bluestone::ExchangeConfig cfg_;
    std::unique_ptr<FIX::SessionSettings> settings_;
    std::unique_ptr<FIX::FileStoreFactory> store_factory_;
    std::unique_ptr<FIX::FileLogFactory> log_factory_;
    std::unique_ptr<FIX::ThreadedSocketInitiator> initiator_;
    // --- ----

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

    // --- FIX::MessageCracker Callbacks ---
    void onMessage(const FIX44::ExecutionReport& message,
                   const FIX::SessionID& sessionID) override;
    void onMessage(const FIX44::MarketDataSnapshotFullRefresh& message,
                   const FIX::SessionID& sessionID) override;

   public:
    explicit LMAXConnector(const bluestone::ExchangeConfig& cfg);

    ~LMAXConnector() override;
    LMAXConnector(const LMAXConnector&);
    LMAXConnector(LMAXConnector&&) noexcept;
    LMAXConnector& operator=(const LMAXConnector&);
    LMAXConnector& operator=(LMAXConnector&&) noexcept;

    // --- IExchangeConnector Implementations ---
    void connect() override;
    void reconnect() override;
    void disconnect() override;
    void subscribe_market_data(int req_id, const std::string& symbol) override;
  };
}  // namespace bluestone
   //
#endif

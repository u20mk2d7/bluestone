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

#include "core/trade_queue.hpp"
#include "gateway/interface/i_exchange_connector.hpp"
#include "utils/config_loader.hpp"

namespace bluestone {
  class LMAXConnector : public bluestone::IExchangeConnector,
                        public FIX::Application,
                        public FIX::MessageCracker {
    //
   private:
    // --- Encapsulated QuickFIX State ---
    bluestone::ExchangeConfig cfg_;
    std::shared_ptr<bluestone::TradeQueue> queue_;
    std::unique_ptr<FIX::SessionSettings> settings_;
    std::unique_ptr<FIX::FileStoreFactory> store_factory_;
    std::unique_ptr<FIX::FileLogFactory> log_factory_;
    std::unique_ptr<FIX::ThreadedSocketInitiator> initiator_;
    //

   public:
    explicit LMAXConnector(const bluestone::ExchangeConfig& cfg,
                           std::shared_ptr<bluestone::TradeQueue> queue);

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
    //
    // Notification of a session begin created
    void onCreate(const FIX::SessionID& sessionID) override;
    /// Notification of a session successfully logging on
    void onLogon(const FIX::SessionID& sessionID) override;
    /// Notification of a session logging off or disconnecting
    void onLogout(const FIX::SessionID& sessionID) override;
    // Notification of admin message being sent to target
    void toAdmin(FIX::Message& message,
                 const FIX::SessionID& sessionID) override;
    // Notification of app message
    void fromAdmin(const FIX::Message& message,
                   const FIX::SessionID& sessionID) override;
    void toApp(FIX::Message& message, const FIX::SessionID& sessionID) override;
    void fromApp(const FIX::Message& message,
                 const FIX::SessionID& sessionID) override;

    // --- FIX::MessageCracker Callbacks ---
    void onMessage(const FIX44::ExecutionReport& message,
                   const FIX::SessionID& sessionID) override;
    void onMessage(const FIX44::MarketDataSnapshotFullRefresh& msg,
                   const FIX::SessionID& sessionID) override;
  };  // class LMAXConnector
}  // namespace bluestone

#endif  // LMAX_CONNECTOR_HPP

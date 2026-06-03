#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <quickfix/Application.h>
#include <quickfix/FileLog.h>
#include <quickfix/FileStore.h>
#include <quickfix/Message.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/SocketInitiator.h>
#include <quickfix/Values.h>

#include <iostream>
#include <memory>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "core/exchange/i_exchange_connector.hpp"
#include "core/trade_engine.hpp"
#include "gateways/binance/binance_connector.hpp"
#include "gateways/binance/fix_binance_connector.hpp"
#include "utils/event_loop_worker.hpp"
#include "utils/utils_cfg.hpp"
#include "utils/utils_cli.hpp"

// Custom Application class overriding QuickFIX virtual methods
class CTraderClient : public FIX::Application {
 private:
  std::string m_password;

 public:
  explicit CTraderClient(std::string password)
      : m_password(std::move(password)) {}

  // Called when a session is successfully logged on to the broker
  void onLogon(const FIX::SessionID& sessionID) override {
    std::cout << "\n[+] SUCCESSFULLY LOGGED ON TO CTRADER! Session: "
              << sessionID << std::endl;
    std::cout << "[+] Ready to subscribe to market data (QUOTE).\n"
              << std::endl;
  }

  // Called when a session is logged out or disconnected
  void onLogout(const FIX::SessionID& sessionID) override {
    std::cout << "[-] Logged out from session: " << sessionID << std::endl;
  }

  // Crucial: Intercept administrative messages before they leave your machine
  void toAdmin(FIX::Message& message,
               const FIX::SessionID& sessionID) override {
    FIX::MsgType msgType;
    message.getHeader().getField(msgType);

    // If the outgoing message is a Logon (35=A), inject the cTrader Password
    if (msgType == FIX::MsgType_Logon) {
      std::cout
          << "[*] Modifying Logon message: Injecting password into Tag 554..."
          << std::endl;

      // Tag 554 = Password (Mandatory for cTrader authentication)
      message.setField(FIX::StringField(554, m_password));
    }
  }

  // Administrative responses coming back from the broker (e.g. Heartbeats,
  // Logon acks)
  void fromAdmin(const FIX::Message& message, const FIX::SessionID&) override
      throw(FIX::FieldNotFound, FIX::IncorrectDataFormat,
            FIX::IncorrectTagValue, FIX::RejectLogon) {}

  // Application messages leaving your machine (like market data requests)
  void toApp(FIX::Message&, const FIX::SessionID&) override
      throw(FIX::DoNotSend) {}

  // Incoming application data (This is where your real-time QUOTE data
  // arrives!)
  void fromApp(const FIX::Message& message,
               const FIX::SessionID& sessionID) override
      throw(FIX::FieldNotFound, FIX::IncorrectDataFormat,
            FIX::IncorrectTagValue, FIX::UnsupportedMessageType) {
    std::cout << "[Incoming FIX Message]: " << message.toString() << std::endl;
  }

  void onCreate(const FIX::SessionID&) override {}
};

int main() {
  try {
    // 1. Load session configuration file
    FIX::SessionSettings settings("initiator.cfg");

    // 2. Instantiate our custom cTrader application logic
    // Replace with the actual password string provided by cTrader
    std::string ctraderPassword = "5814863";
    CTraderClient application(ctraderPassword);

    // 3. Setup QuickFIX storage and logging systems
    FIX::FileStoreFactory storeFactory(settings);
    FIX::FileLogFactory logFactory(settings);

    // 4. Initialize the network connection wrapper
    std::cout << "[*] Initializing QuickFIX Socket Initiator..." << std::endl;
    FIX::SocketInitiator initiator(application, storeFactory, settings,
                                   logFactory);

    // 5. Start the engine loop asynchronously
    initiator.start();
    std::cout
        << "[+] Engine started. Attempting handshake with cTrader network..."
        << std::endl;

    // Keep main thread alive while background threads handle processing
    std::cout << "Press Enter to shut down the client safely..." << std::endl;
    std::cin.get();

    // 6. Graceful teardown
    std::cout << "[*] Stopping initiator loop..." << std::endl;
    initiator.stop();
    return 0;
  } catch (std::exception& e) {
    std::cerr << "[-] Fatal Error Exception caught: " << e.what() << std::endl;
    return 1;
  }
}

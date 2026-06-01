#pragma once
#ifndef BINANCE_FIX_APPLICATION_HPP
#define BINANCE_FIX_APPLICATION_HPP

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Values.h>

#include <string>

#include "auth.hpp"

namespace Bluestone {

  class BinanceFixApplication : public FIX::Application,
                                public FIX::MessageCracker {
   public:
    BinanceFixApplication(const std::string& api_key,
                          const std::string& private_key_pem)
        : api_key_(api_key), signer_(private_key_pem) {}

    void onCreate(const FIX::SessionID& sessionID) override {}
    void onLogon(const FIX::SessionID& sessionID) override {}
    void onLogout(const FIX::SessionID& sessionID) override {}

    void toApp(FIX::Message& message,
               const FIX::SessionID& sessionID) noexcept(false) override {}
    void fromAdmin(const FIX::Message& message,
                   const FIX::SessionID& sessionID) noexcept(false) override {}

    void fromApp(const FIX::Message& message,
                 const FIX::SessionID& sessionID) noexcept(false) override {
      crack(message, sessionID);
    }

    void toAdmin(FIX::Message& message,
                 const FIX::SessionID& sessionID) override {
      if (message.getHeader().getField(FIX::FIELD::MsgType) ==
          FIX::MsgType_Logon) {
        message.setField(FIX::Username(api_key_));

        std::string senderCompID =
            message.getHeader().getField(FIX::FIELD::SenderCompID);
        std::string targetCompID =
            message.getHeader().getField(FIX::FIELD::TargetCompID);
        std::string msgSeqNum =
            message.getHeader().getField(FIX::FIELD::MsgSeqNum);
        std::string sendingTime =
            message.getHeader().getField(FIX::FIELD::SendingTime);
        std::string payload_to_sign = std::string("A") + '\x01' + senderCompID +
                                      '\x01' + targetCompID + '\x01' +
                                      msgSeqNum + '\x01' + sendingTime;
        std::cout << "\n[Auth] Signing Payload: A|" << senderCompID << "|"
                  << targetCompID << "|" << msgSeqNum << "|" << sendingTime
                  << "\n";
        std::string signature = signer_.sign(payload_to_sign);

        message.setField(FIX::RawDataLength(signature.length()));
        message.setField(FIX::RawData(signature));
      }
    }

   private:
    std::string api_key_;
    bluestone::binance::Ed25519Signer signer_;
  };

}  // namespace Bluestone

#endif

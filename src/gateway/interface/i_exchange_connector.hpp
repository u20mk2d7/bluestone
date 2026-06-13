#pragma once
#ifndef IEXCHANGECONNECTOR_HPP
#define IEXCHANGECONNECTOR_HPP

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

namespace bluestone {
  class IExchangeConnector {
   public:
    virtual ~IExchangeConnector() = default;
    IExchangeConnector() = default;
    IExchangeConnector(const IExchangeConnector&) = delete;
    IExchangeConnector& operator=(IExchangeConnector) = delete;
    IExchangeConnector(IExchangeConnector&&) = delete;
    IExchangeConnector& operator=(IExchangeConnector&&) = delete;

    virtual void connect() = 0;
    virtual void reconnect() = 0;
    virtual void disconnect() = 0;
    virtual void subscribe_market_data(int req_id,
                                       const std::string& symbol) = 0;
  };
}  // namespace bluestone
#endif  // IEXCHANGECONNECTOR_HPP

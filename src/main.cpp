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

#include "core/exchange/i_exchange_connector.hpp"
#include "core/trade_engine.hpp"
#include "gateways/binance/binance_connector.hpp"
#include "utils/cli_util.hpp"
#include "utils/event_loop_worker.hpp"

int main(int argc, char** argv) {
  bluestone::CLI::parse(argc, argv);
  return 0;
}

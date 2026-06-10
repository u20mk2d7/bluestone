#include <quickfix/FileLog.h>
#include <quickfix/FileStore.h>
#include <quickfix/SessionSettings.h>
#include <quickfix/SocketInitiator.h>
#include <quickfix/ThreadedSocketInitiator.h>

#include <iostream>

#include "gateways/lmax/lmax_connector.hpp"

int main(int argc, char** argv) {
  try {
    // 1. Load the configuration file
    FIX::SessionSettings settings(".env/lmax.cfg");

    // 2. Instantiate your application
    LMAXApplication application;

    // 3. Set up storage and logging
    FIX::FileStoreFactory storeFactory(settings);
    FIX::FileLogFactory logFactory(settings);

    // 4. Initialize the Initiator
    FIX::ThreadedSocketInitiator initiator(application, storeFactory, settings,
                                           logFactory);

    // 5. Start the connection
    initiator.start();

    std::cout << "Initiator started. Press ENTER to stop." << std::endl;
    std::cin.get();

    initiator.stop();
    return 0;

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}

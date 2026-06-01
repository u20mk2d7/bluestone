#include "event_loop_worker.hpp"

namespace Bluestone {

  EventLoopWorker::~EventLoopWorker() {
    work_.reset();
    ioc_.stop();
    if (thread_.joinable()) {
      thread_.join();
    }
  }

}  // namespace Bluestone

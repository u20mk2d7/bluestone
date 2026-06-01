#include "event_loop_worker.hpp"

namespace bluestone {

  EventLoopWorker::~EventLoopWorker() {
    work_.reset();
    ioc_.stop();
    if (thread_.joinable()) {
      thread_.join();
    }
  }

}  // namespace bluestone

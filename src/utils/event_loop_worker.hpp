#pragma once
#ifndef BLUESTONE_UTILS_EVENT_LOOP_WORKER_HPP
#define BLUESTONE_UTILS_EVENT_LOOP_WORKER_HPP

#include <boost/asio.hpp>
#include <thread>

namespace bluestone {

  class EventLoopWorker {
   private:
    boost::asio::io_context ioc_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work_;
    std::thread thread_;

   public:
    // Delete copy/move semantics to enforce strict, single-owner lifecycle
    EventLoopWorker(const EventLoopWorker&) = delete;
    EventLoopWorker& operator=(const EventLoopWorker&) = delete;
    EventLoopWorker(EventLoopWorker&&) = delete;
    EventLoopWorker& operator=(EventLoopWorker&&) = delete;

    // Strict declarations only
    explicit EventLoopWorker(int core_id);
    ~EventLoopWorker();
  };

}  // namespace bluestone

#endif  // BLUESTONE_UTILS_EVENT_LOOP_WORKER_HPP

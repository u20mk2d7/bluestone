#pragma once
#ifndef BLUESTONE_UTILS_EVENT_LOOP_WORKER_HPP
#define BLUESTONE_UTILS_EVENT_LOOP_WORKER_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <thread>

#ifdef __linux__
#include <pthread.h>
#elif defined(__APPLE__)
#include <mach/thread_act.h>
#include <mach/thread_policy.h>
#endif

namespace bluestone {

  class EventLoopWorker {
   private:
    boost::asio::io_context ioc_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work_;
    std::thread thread_;

   public:
    EventLoopWorker(const EventLoopWorker&) = delete;
    EventLoopWorker& operator=(const EventLoopWorker&) = delete;
    EventLoopWorker(EventLoopWorker&&) = delete;
    EventLoopWorker& operator=(EventLoopWorker&&) = delete;

    explicit EventLoopWorker(int core_id)
        : work_(boost::asio::make_work_guard(ioc_)) {
      thread_ = std::thread([this, core_id]() {
      // CPU Affinity Pinning
#if defined(__linux__)
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
        std::cout << "[EventLoop] Pinned to Linux Core " << core_id << "\n";

#elif defined(__APPLE__)
        thread_affinity_policy_data_t policy = {core_id};
        thread_policy_set(pthread_mach_thread_np(pthread_self()),
                          THREAD_AFFINITY_POLICY,
                          reinterpret_cast<thread_policy_t>(&policy),
                          THREAD_AFFINITY_POLICY_COUNT);
        std::cout << "[EventLoop] Assigned Apple Affinity Tag " << core_id
                  << "\n";
#endif

        ioc_.run();
        std::cout << "[EventLoop] Core " << core_id << " shutting down.\n";
      });
    }

    ~EventLoopWorker();
    boost::asio::io_context& get_ioc() { return ioc_; }
  };

}  // namespace bluestone

#endif  // BLUESTONE_UTILS_EVENT_LOOP_WORKER_HPP

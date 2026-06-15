#include "event_loop_worker.hpp"

#include <iostream>

// OS-specific headers isolated to the compilation unit
#if defined(__linux__)
#include <pthread.h>
#elif defined(__APPLE__)
#include <mach/thread_act.h>
#include <mach/thread_policy.h>
#endif

namespace bluestone {

  // 1. Constructor Implementation (Thread Pinning)
  EventLoopWorker::EventLoopWorker(int core_id)
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

      // Ignite the Asio Event Loop
      ioc_.run();
      std::cout << "[EventLoop] Core " << core_id << " shutting down.\n";
    });
  }

  // 2. Destructor Implementation (Safe Shutdown)
  EventLoopWorker::~EventLoopWorker() {
    work_.reset();  // Drop the work guard
    ioc_.stop();    // Stop the IO context
    if (thread_.joinable()) {
      thread_.join();  // Safely wait for the thread to exit
    }
  }

}  // namespace bluestone

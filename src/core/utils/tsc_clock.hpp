#pragma once
#ifndef BLUESTONE_CORE_UTILS_TSC_CLOCK_HPP
#define BLUESTONE_CORE_UTILS_TSC_CLOCK_HPP

#include <cstdint>

// CPU-specific intrinsic headers
#if defined(__x86_64__) || defined(_M_X64) || defined(__i386__)
#include <x86intrin.h>
#elif defined(__aarch64__)
// ARM/Apple Silicon support
#endif

namespace bluestone {
namespace utils {

class TSCClock {
  public:
  // Forces the compiler to inline this directly into the execution path
  // Zero function-call overhead.
  [[nodiscard]] static inline uint64_t now() noexcept {
#if defined(__x86_64__) || defined(_M_X64)
    // Intel/AMD: Read the Time Stamp Counter directly from the CPU
    // __rdtsc() executes in ~10-20 CPU cycles (insanely fast)
    unsigned int aux;
    return __rdtscp(&aux);
#elif defined(__aarch64__)
    // ARM / Apple Silicon (M1/M2/M3) equivalent
    uint64_t val;
    asm volatile("mrs %0, cntvct_el0" : "=r"(val));
    return val;
#else
    // Fallback for unsupported architectures (will be slower)
    return 0;
#endif
  }
};

}  // namespace utils
}  // namespace bluestone

#endif  // BLUESTONE_CORE_UTILS_TSC_CLOCK_HPP
#pragma once

#include <cassert>
#include <chrono>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>

namespace groebner::testing {

class ScopedTimer {
  using Clock = std::chrono::steady_clock;

 public:
  explicit ScopedTimer(std::string label, std::ostream* out = &std::cout)
      : label_(std::move(label)), out_(out), start_(Clock::now()) {}

  ~ScopedTimer() {
    const auto end = Clock::now();
    const double ms =
        std::chrono::duration<double, std::milli>(end - start_).count();
    *out_ << label_ << ": " << ms << " ms\n";
  }

  ScopedTimer(const ScopedTimer&) = delete;
  ScopedTimer(ScopedTimer&&) = delete;
  ScopedTimer& operator=(const ScopedTimer&) = delete;
  ScopedTimer& operator=(ScopedTimer&&) = delete;

 private:
  std::string label_;
  std::ostream* out_;
  Clock::time_point start_;
};

}  // namespace groebner::testing

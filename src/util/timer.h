#pragma once

#include <chrono>

namespace ann {

class Timer {
public:
    Timer() noexcept : Started_(Clock::now()) {}

    void Restart() noexcept {
        Started_ = Clock::now();
    }

    std::chrono::nanoseconds Elapsed() const noexcept {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - Started_);
    }

private:
    using Clock = std::chrono::steady_clock;

private:
    Clock::time_point Started_;
};

} // namespace ann

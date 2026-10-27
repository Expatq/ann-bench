#include "thread_pool.h"

#include <algorithm>
#include <stdexcept>

namespace ann {

ThreadPool::ThreadPool(std::size_t threadCount)
    : Launch_(1), Start_(ParticipantCount(threadCount), StartTimer{Timer_}), Finish_(ParticipantCount(threadCount)),
      Elapsed_(threadCount), StopSource_(std::nostopstate), Task_(nullptr), LaunchFailed_(false), Stopping_(false) {
    Workers_.reserve(threadCount);
    try {
        for (std::size_t workerId = 0; workerId < threadCount; ++workerId) {
            Workers_.emplace_back([this, workerId] { RunWorker(workerId); });
        }
    } catch (...) {
        LaunchFailed_ = true;
        Launch_.count_down();
        throw;
    }
    Launch_.count_down();
}

ThreadPool::~ThreadPool() {
    Stopping_ = true;
    Start_.arrive_and_wait();
}

std::size_t ThreadPool::Size() const noexcept {
    return Workers_.size();
}

std::chrono::nanoseconds ThreadPool::RunOnEachWorker(const Task& task) {
    StopSource_ = std::stop_source();
    Error_ = nullptr;
    Task_ = &task;

    Start_.arrive_and_wait();
    Finish_.arrive_and_wait();

    Task_ = nullptr;
    if (Error_) {
        std::rethrow_exception(Error_);
    }
    return *std::max_element(Elapsed_.begin(), Elapsed_.end());
}

void ThreadPool::StartTimer::operator()() noexcept {
    RunTimer.Restart();
}

std::ptrdiff_t ThreadPool::ParticipantCount(std::size_t threadCount) {
    const auto maximum = std::min(std::barrier<StartTimer>::max(), std::barrier<>::max());
    if (threadCount == 0 || threadCount >= static_cast<std::size_t>(maximum)) {
        throw std::invalid_argument("thread count must be positive and below the barrier limit");
    }
    return static_cast<std::ptrdiff_t>(threadCount) + 1;
}

void ThreadPool::RunWorker(std::size_t workerId) {
    Launch_.wait();
    if (LaunchFailed_) {
        return;
    }

    for (;;) {
        Start_.arrive_and_wait();
        if (Stopping_) {
            return;
        }
        ExecuteTask(workerId);
        Elapsed_[workerId] = Timer_.Elapsed();
        Finish_.arrive_and_wait();
    }
}

void ThreadPool::ExecuteTask(std::size_t workerId) {
    try {
        (*Task_)(workerId, StopSource_.get_token());
    } catch (...) {
        {
            std::lock_guard lock(ErrorMutex_);
            if (!Error_) {
                Error_ = std::current_exception();
            }
        }
        StopSource_.request_stop();
    }
}

} // namespace ann

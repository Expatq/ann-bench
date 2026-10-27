#pragma once

#include <util/timer.h>

#include <barrier>
#include <chrono>
#include <cstddef>
#include <exception>
#include <functional>
#include <latch>
#include <mutex>
#include <stop_token>
#include <thread>
#include <vector>

namespace ann {

class ThreadPool {
public:
    explicit ThreadPool(std::size_t threadCount);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    std::size_t Size() const noexcept;

    std::chrono::nanoseconds RunOnEachWorker(const std::function<void(std::size_t, std::stop_token)>& task);

private:
    using Task = std::function<void(std::size_t, std::stop_token)>;

    struct StartTimer {
        Timer& RunTimer;

        void operator()() noexcept;
    };

private:
    static std::ptrdiff_t ParticipantCount(std::size_t threadCount);

private:
    void RunWorker(std::size_t workerId);
    void ExecuteTask(std::size_t workerId);

private:
    std::latch Launch_;
    Timer Timer_;
    
    std::barrier<StartTimer> Start_;
    std::barrier<> Finish_;
    std::vector<std::chrono::nanoseconds> Elapsed_;
    std::stop_source StopSource_;

    const Task* Task_;
    std::exception_ptr Error_;
    std::mutex ErrorMutex_;
    bool LaunchFailed_;
    bool Stopping_;
    
    std::vector<std::jthread> Workers_;
};

} // namespace ann

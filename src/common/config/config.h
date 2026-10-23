#pragma once

#include <src/common/metrics.h>

#include <cstddef>
#include <cstdint>

namespace ann {

struct BuildConfig {
    std::uint64_t Seed;
    MetricKind Metric;
    std::uint16_t Threads;
};

struct SearchConfig {
    std::uint16_t NeighborsPerQuery;
    std::uint16_t Threads;
};

struct BenchmarkConfig {
    std::uint16_t QueryThreads;
    std::size_t WarmupQueriesPerWorker;
    std::size_t Repetitions;
    std::uint64_t Seed;
};

struct RunConfig {
    BuildConfig Build;
    SearchConfig Search;
    BenchmarkConfig Measurement;
};

} // namespace ann

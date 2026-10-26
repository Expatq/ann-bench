#pragma once

#include <common/metrics.h>

#include <cstddef>
#include <cstdint>

namespace ann {

struct DatasetConfig {
    MetricKind Metric;
    std::uint32_t Dim;
    std::uint32_t BaseCount;
    std::uint32_t QueryCount;
};

struct BuildConfig {
    std::uint64_t Seed;
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

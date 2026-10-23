#pragma once

#include "metrics.h"

#include <cstddef>
#include <cstdint>

namespace ann {

struct BuildConfig {
    std::uint64_t Seed;
    MetricKind Metric;
    std::uint8_t Threads;
};

struct SearchConfig {
    std::uint16_t NeighborsPerQuery;
    std::uint8_t Threads;
};

} // namespace ann

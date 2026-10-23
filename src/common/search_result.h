#pragma once

#include "types.h"

#include <cstddef>
#include <span>
#include <vector>

namespace ann {

// Stores ANN or reference IDs in query-major order
struct SearchResult {
    std::size_t QueryCount;
    std::size_t NeighborsPerQuery;
    std::vector<VectorId> Ids;

    void Resize(std::size_t queryCount, std::size_t neighborsPerQuery);

    std::span<VectorId> Row(std::size_t query);
    std::span<const VectorId> Row(std::size_t query) const;
};

} // namespace ann

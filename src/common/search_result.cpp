#include "search_result.h"

namespace ann {

void SearchResult::Resize(std::size_t queryCount, std::size_t neighborsPerQuery) {
    Ids.assign(queryCount * neighborsPerQuery, MissingVectorId);
    QueryCount = queryCount;
    NeighborsPerQuery = neighborsPerQuery;
}

std::span<VectorId> SearchResult::Row(std::size_t query) {
    return std::span(Ids).subspan(query * NeighborsPerQuery, NeighborsPerQuery);
}

std::span<const VectorId> SearchResult::Row(std::size_t query) const {
    return std::span(Ids).subspan(query * NeighborsPerQuery, NeighborsPerQuery);
}

} // namespace ann

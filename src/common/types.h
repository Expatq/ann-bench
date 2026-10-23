#pragma once

#include <nlohmann/json.hpp>

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace ann {

using Json = nlohmann::json;
using VectorId = std::int64_t;
inline constexpr VectorId MissingVectorId = -1;

enum class BackendKind : std::uint8_t {
    Faiss,
    Scann,
    Hnsw,
    Spann
};

enum class RunMode : std::uint8_t {
    Build,
    Search
};

enum class VectorEncoding : std::uint8_t {
    Float32,
    SQ8,
    PQ
};

// Borrows contiguous row-major float32 vectors. The owner keeps them alive.
// Row and Slice require valid indices.
// Neither allocates nor transforms data.
struct MatrixView {
    const float* Data;
    std::size_t Rows;
    std::size_t Dim;

    std::span<const float> Row(std::size_t row) const;
    MatrixView Slice(std::size_t begin, std::size_t count) const;
};

// Stores ANN or reference IDs in query-major order
struct SearchResult {
    std::size_t QueryCount;
    std::size_t NeighborsPerQuery;
    std::vector<VectorId> Ids;

    void Resize(std::size_t queryCount, std::size_t neighborsPerQuery) {
        Ids.assign(queryCount * neighborsPerQuery, MissingVectorId);
        QueryCount = queryCount;
        NeighborsPerQuery = neighborsPerQuery;
    }

    std::span<VectorId> Row(std::size_t query) {
        return std::span(Ids).subspan(query * NeighborsPerQuery, NeighborsPerQuery);
    }

    std::span<const VectorId> Row(std::size_t query) const {
        return std::span(Ids).subspan(query * NeighborsPerQuery, NeighborsPerQuery);
    }
};

} // namespace ann

#pragma once

#include "matrix.h"
#include "types.h"

#include <common/config/config.h>

#include <cstddef>
#include <filesystem>
#include <span>

namespace ann {

struct IndexInfo {
    std::size_t Size;
    std::size_t Dim;
    MetricKind Metric;
    VectorEncoding Encoding;
};

// Native usage pipeline: Build/Load -> PrepareSearch -> Search (concurent by design) -> [Save]
class IBackend {
public:
    using Matrix = Matrix<float>;

public:
    virtual ~IBackend() = default;

    // Takes ownership of the original float32 base, row numbers become stable IDs.
    // Includes sampling from the base, training, encoding and index construction.
    // After return, the backend owns all data needed to serve the index.
    virtual void Build(Matrix base, MetricKind metric, const BuildConfig& config) = 0;

    // Alternative to Build. Load the complete index and any rerank vectors into RAM.
    virtual void Load(const std::filesystem::path& directory) = 0;

    // Persist the finished index without further construction.
    virtual void Save(const std::filesystem::path& directory) const = 0;

    // Apply search parameters and prepare query-independent state.
    virtual void PrepareSearch(const SearchConfig& config) = 0;

    // One query per call: query.size() == index dimension; ids.size() == NeighborsPerQuery.
    // Returns unique ids ordered by backend's scores, best first.
    // Pad any missing suffix with MissingVectorId.
    // Includes all query-dependent work, including encoding, lookup tables and reranking.
    virtual void Search(std::span<const float> query, std::span<VectorId> ids) const = 0;

    // Can be called after Build or Load. Describes the index.
    virtual IndexInfo GetInfo() const = 0;
};

} // namespace ann

#pragma once

#include "config.h"
#include "types.h"

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
    virtual ~IBackend() = default;

    // Input rows become stable IDs [0, base.Rows).
    // Training may be empty if unused.
    // After return, the backend owns all data needed to serve the index.
    virtual void Build(MatrixView base, MatrixView training, const BuildConfig &config) = 0;

    // Alternative to Build. Load the complete index and any rerank vectors into RAM.
    virtual void Load(const std::filesystem::path &directory) = 0;

    // Persist the finished index without further construction.
    virtual void Save(const std::filesystem::path &directory) const = 0;

    // Apply search parameters and prepare query-independent state.
    virtual void PrepareSearch(const SearchConfig &config) = 0;

    // One query per call: query.size() == index dimension; ids.size() == NeighborsPerQuery.
    // Returns unique ids ordered by backend's scores, best first.
    // Pad any missing suffix with MissingVectorId.
    // Includes all query-dependent work, including encoding, lookup tables and reranking.
    virtual void Search(std::span<const float> query, std::span<VectorId> ids) const = 0;

    // Can be called after Build or Load. Describes the index.
    virtual IndexInfo GetInfo() const = 0;
};

} // namespace ann

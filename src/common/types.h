#pragma once

#include <nlohmann/json.hpp>

#include <cstdint>

namespace ann {

using Json = nlohmann::json;
using VectorId = std::int64_t;
inline constexpr VectorId MissingVectorId = -1;

enum class BackendKind : std::uint8_t {
    Faiss,
    Scann,
    Hnsw,
    Spann,
    FullScan,
};

enum class RunMode : std::uint8_t {
    Build,
    Search,
};

enum class VectorEncoding : std::uint8_t {
    Float32,
    SQ8,
    PQ,
};

} // namespace ann

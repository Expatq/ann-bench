#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

namespace ann {

using VectorId = std::int64_t;
inline constexpr VectorId MissingVectorId = -1;

enum class VectorEncoding : std::uint8_t {
    Float32,
    SQ8,
    PQ
};

// Borrows contiguous row-major float32 vectors. The owner keeps them alive.
// Row and Slice require valid indices.
// Neither allocates nor transforms data.
struct MatrixView {
    const float *Data;
    std::size_t Rows;
    std::size_t Dim;

    std::span<const float> Row(std::size_t row) const;
    MatrixView Slice(std::size_t begin, std::size_t count) const;
};

} // namespace ann

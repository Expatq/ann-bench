#include "types.h"

namespace ann {

std::span<const float> MatrixView::Row(std::size_t row) const {
    return {Data + row * Dim, Dim};
}

MatrixView MatrixView::Slice(std::size_t startRow, std::size_t count) const {
    return {Data ? Data + startRow * Dim : nullptr, count, Dim};
}

} // namespace ann

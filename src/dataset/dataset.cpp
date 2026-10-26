#include "dataset.h"

#include <src/io/file.h>

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <utility>

namespace ann {

Dataset::Dataset(std::filesystem::path directory, const DatasetConfig& config)
    : Directory_(std::move(directory)), Config_(config) {
    if (Config_.Dim == 0) {
        throw std::invalid_argument("dataset dimension must be positive");
    }
}

MetricKind Dataset::GetMetric() const {
    return Config_.Metric;
}

Dataset::Matrix Dataset::LoadBase() const {
    return LoadMatrix("base.bin", Config_.BaseCount);
}

Dataset::Matrix Dataset::LoadQueries() const {
    return LoadMatrix("queries.bin", Config_.QueryCount);
}

Dataset::Matrix Dataset::LoadMatrix(const char* filename, std::uint32_t rows) const {
    const File file(Directory_ / filename);

    const auto bytes = file.Size();
    const auto rowBytes = static_cast<std::size_t>(Config_.Dim) * sizeof(float);
    if (bytes % rowBytes != 0 || bytes / rowBytes != rows) {
        throw std::runtime_error("dataset file size does not match its configuration: " +
                                 (Directory_ / filename).string());
    }

    Matrix matrix(rows, Config_.Dim);
    const auto data = matrix.Data();
    file.Read(0, {reinterpret_cast<std::uint8_t*>(data.data()), data.size_bytes()});

    return matrix;
}

} // namespace ann

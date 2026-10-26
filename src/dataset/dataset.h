#pragma once

#include <src/common/config/config.h>
#include <src/common/matrix.h>

#include <cstdint>
#include <filesystem>

namespace ann {

class IDataset {
public:
    using Matrix = Matrix<float>;

public:
    virtual ~IDataset() = default;

    virtual MetricKind GetMetric() const = 0;

    virtual Matrix LoadBase() const = 0;
    virtual Matrix LoadQueries() const = 0;
};

class Dataset : public IDataset {
public:
    Dataset(std::filesystem::path directory, const DatasetConfig& config);

    MetricKind GetMetric() const override;
    Matrix LoadBase() const override;
    Matrix LoadQueries() const override;

private:
    Matrix LoadMatrix(const char* filename, std::uint32_t rows) const;

private:
    std::filesystem::path Directory_;
    DatasetConfig Config_;
};

} // namespace ann

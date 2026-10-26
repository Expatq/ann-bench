#pragma once

#include <common/backend.h>

#include <memory>
#include <optional>

namespace ann {

class FullScanBackend : public IBackend {
public:
    void Build(Matrix base, MetricKind metric, const BuildConfig& config) override;

    void Load(const std::filesystem::path& directory) override;

    void Save(const std::filesystem::path& directory) const override;

    void PrepareSearch(const SearchConfig& config) override;

    void Search(std::span<const float> query, std::span<VectorId> ids) const override;

    IndexInfo GetInfo() const override;

private:
    struct Neighbor {
        double Value;
        VectorId Id;
    };

private:
    std::optional<Matrix> Base_;
    std::unique_ptr<IMetric> Metric_;
    std::optional<SearchConfig> SearchConfig_;
};

} // namespace ann

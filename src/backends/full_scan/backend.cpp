#include "backend.h"
#include "common/metrics.h"

#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>

namespace ann {

void FullScanBackend::Build(Matrix base, MetricKind metric, const BuildConfig&) {
    Base_ = std::move(base);
    Metric_ = IMetric::Create(metric);
    SearchConfig_.reset();
}

void FullScanBackend::Load(const std::filesystem::path&) {
    throw std::logic_error("FullScanBackend::Load is not supported; use Build");
}

void FullScanBackend::Save(const std::filesystem::path&) const {
    throw std::logic_error("FullScanBackend::Save is not supported");
}

void FullScanBackend::PrepareSearch(const SearchConfig& config) {
    if (config.NeighborsPerQuery == 0) {
        throw std::invalid_argument("FullScanBackend requires a positive NeighborsPerQuery");
    }
    if (config.Threads != 1) {
        throw std::invalid_argument("FullScanBackend requires SearchConfig::Threads == 1");
    }
    SearchConfig_ = config;
}

void FullScanBackend::Search(std::span<const float> query, std::span<VectorId> ids) const {
    if (!SearchConfig_) {
        throw std::logic_error("FullScanBackend::Search requires PrepareSearch");
    }

    const auto base = Base_->View();
    if (query.size() != base.Dim || ids.size() != SearchConfig_->NeighborsPerQuery) {
        throw std::invalid_argument("FullScanBackend::Search received incompatible buffer sizes");
    }

    const auto compare = [this](const Neighbor& left, const Neighbor& right) {
        if (left.Value == right.Value) {
            return left.Id < right.Id;
        }
        return Metric_->Compare(left.Value, right.Value);
    };

    const auto count = std::min(base.Rows, ids.size());
    std::vector<Neighbor> heap;
    heap.reserve(count);

    for (VectorId row = 0; row < static_cast<VectorId>(base.Rows); ++row) {
        const Neighbor candidate{Metric_->Calc(query, base.Row(row)), row};
        if (heap.size() < count) {
            heap.push_back(candidate);
            std::push_heap(heap.begin(), heap.end(), compare);
        } else if (compare(candidate, heap.front())) {
            std::pop_heap(heap.begin(), heap.end(), compare);
            heap.back() = candidate;
            std::push_heap(heap.begin(), heap.end(), compare);
        }
    }

    std::sort_heap(heap.begin(), heap.end(), compare);
    std::fill(ids.begin(), ids.end(), MissingVectorId);
    for (std::size_t i = 0; i < heap.size(); ++i) {
        ids[i] = heap[i].Id;
    }
}

IndexInfo FullScanBackend::GetInfo() const {
    const auto base = Base_->View();
    return {base.Rows, base.Dim, Metric_->GetType(), VectorEncoding::Float32};
}

} // namespace ann

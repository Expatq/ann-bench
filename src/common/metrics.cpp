#include "metrics.h"

#include <stdexcept>

namespace ann {

std::unique_ptr<IMetric> IMetric::Create(MetricKind kind) {
    switch (kind) {
    case MetricKind::L2:
        return std::make_unique<L2Metric>();
    case MetricKind::InnerProduct:
        return std::make_unique<InnerProductMetric>();
    }
    throw std::invalid_argument("unknown metric");
}

MetricKind L2Metric::GetType() const {
    return MetricKind::L2;
}

double L2Metric::Calc(std::span<const float> left, std::span<const float> right) const {
    double value = 0;
    for (std::size_t i = 0; i < left.size(); ++i) {
        const double difference = static_cast<double>(left[i]) - static_cast<double>(right[i]);
        value += difference * difference;
    }
    return value;
}

bool L2Metric::Compare(double left, double right) const {
    return left < right;
}

MetricKind InnerProductMetric::GetType() const {
    return MetricKind::InnerProduct;
}

double InnerProductMetric::Calc(std::span<const float> left, std::span<const float> right) const {
    double value = 0;
    for (std::size_t i = 0; i < left.size(); ++i) {
        value += static_cast<double>(left[i]) * static_cast<double>(right[i]);
    }
    return value;
}

bool InnerProductMetric::Compare(double left, double right) const {
    return left > right;
}

} // namespace ann

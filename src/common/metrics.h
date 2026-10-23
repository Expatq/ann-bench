#pragma once

#include <cstdint>
#include <span>

namespace ann {

enum class MetricKind : uint8_t {
    L2,
    InnerProduct
};

class IMetric {
public:
    virtual ~IMetric() = default;

    virtual MetricKind GetType() const = 0;

    virtual double Calc(std::span<const float> left, std::span<const float> right) const = 0;

    // Strict comparison: true when left is better. L2: <; InnerProduct: >.
    virtual bool Compare(double left, double right) const = 0;
};

} // namespace ann

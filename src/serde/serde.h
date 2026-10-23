#pragma once

#include <src/common/config/config.h>
#include <src/common/metrics.h>
#include <src/common/types.h>

#include <concepts>

namespace ann {

template <typename T>
concept SerdeEnum =
    std::same_as<T, BackendKind> || std::same_as<T, RunMode> || std::same_as<T, MetricKind>;

template <typename T>
concept SerdeType = SerdeEnum<T> || std::same_as<T, BuildConfig> || std::same_as<T, SearchConfig> ||
                    std::same_as<T, BenchmarkConfig> || std::same_as<T, RunConfig>;

class Serde {
public:
    template <SerdeType T>
    static Json Serialize(const T& value);

    template <SerdeType T>
    static T Deserialize(const Json& json);

private:
    template <SerdeEnum T>
    static constexpr auto EnumValues();
};

} // namespace ann

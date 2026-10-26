#include "serde.h"

#include <array>
#include <cctype>
#include <stdexcept>
#include <string>
#include <utility>

namespace ann {

template <SerdeEnum T>
constexpr auto Serde::EnumValues() {
    if constexpr (std::same_as<T, BackendKind>) {
        return std::array{std::pair{T::Faiss, "faiss"}, std::pair{T::Scann, "scann"},
                          std::pair{T::Hnsw, "hnsw"}, std::pair{T::Spann, "spann"}};
    } else if constexpr (std::same_as<T, RunMode>) {
        return std::array{std::pair{T::Build, "build"}, std::pair{T::Search, "search"}};
    } else if constexpr (std::same_as<T, MetricKind>) {
        return std::array{std::pair{T::L2, "l2"}, std::pair{T::InnerProduct, "ip"}};
    }
}

template <SerdeType T>
Json Serde::Serialize(const T& value) {
    if constexpr (SerdeEnum<T>) {
        for (const auto& [item, name] : EnumValues<T>()) {
            if (value == item) {
                return name;
            }
        }
        throw std::invalid_argument("unknown enum value");
    } else if constexpr (std::same_as<T, BuildConfig>) {
        return {{"seed", value.Seed}, {"threads", value.Threads}};
    } else if constexpr (std::same_as<T, SearchConfig>) {
        return {{"neighbors_per_query", value.NeighborsPerQuery}, {"threads", value.Threads}};
    } else if constexpr (std::same_as<T, BenchmarkConfig>) {
        return {{"query_threads", value.QueryThreads},
                {"warmup_queries_per_worker", value.WarmupQueriesPerWorker},
                {"repetitions", value.Repetitions},
                {"seed", value.Seed}};
    } else if constexpr (std::same_as<T, RunConfig>) {
        return {{"build", Serialize(value.Build)},
                {"search", Serialize(value.Search)},
                {"measurement", Serialize(value.Measurement)}};
    } else if constexpr (std::same_as<T, DatasetConfig>) {
        return {{"metric", Serialize(value.Metric)},
                {"dim", value.Dim},
                {"base_count", value.BaseCount},
                {"query_count", value.QueryCount}};
    }
}

template <SerdeType T>
T Serde::Deserialize(const Json& json) {
    T value;
    if constexpr (SerdeEnum<T>) {
        auto name = json.get<std::string>();
        for (auto& letter : name) {
            letter = static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));
        }
        std::string expected;
        for (const auto& [item, label] : EnumValues<T>()) {
            if (name == label) {
                return item;
            }
            if (!expected.empty()) {
                expected += ", ";
            }
            expected += label;
        }
        throw std::invalid_argument("unknown enum value: " + name + "; expected " + expected);
    } else if constexpr (std::same_as<T, BuildConfig>) {
        json.at("seed").get_to(value.Seed);
        json.at("threads").get_to(value.Threads);
    } else if constexpr (std::same_as<T, SearchConfig>) {
        json.at("neighbors_per_query").get_to(value.NeighborsPerQuery);
        json.at("threads").get_to(value.Threads);
    } else if constexpr (std::same_as<T, BenchmarkConfig>) {
        json.at("query_threads").get_to(value.QueryThreads);
        json.at("warmup_queries_per_worker").get_to(value.WarmupQueriesPerWorker);
        json.at("repetitions").get_to(value.Repetitions);
        json.at("seed").get_to(value.Seed);
    } else if constexpr (std::same_as<T, RunConfig>) {
        value.Build = Deserialize<BuildConfig>(json.at("build"));
        value.Search = Deserialize<SearchConfig>(json.at("search"));
        value.Measurement = Deserialize<BenchmarkConfig>(json.at("measurement"));

        if (value.Build.Threads == 0 || value.Search.Threads == 0 ||
            value.Search.NeighborsPerQuery == 0 || value.Measurement.QueryThreads == 0 ||
            value.Measurement.Repetitions == 0) {
            throw std::invalid_argument(
                "threads, neighbors_per_query, query_threads and repetitions must be positive");
        }
    } else if constexpr (std::same_as<T, DatasetConfig>) {
        value.Metric = Deserialize<MetricKind>(json.at("metric"));
        json.at("dim").get_to(value.Dim);
        json.at("base_count").get_to(value.BaseCount);
        json.at("query_count").get_to(value.QueryCount);
    }
    return value;
}

template Json Serde::Serialize<BackendKind>(const BackendKind&);
template BackendKind Serde::Deserialize<BackendKind>(const Json&);

template Json Serde::Serialize<RunMode>(const RunMode&);
template RunMode Serde::Deserialize<RunMode>(const Json&);

template Json Serde::Serialize<MetricKind>(const MetricKind&);
template MetricKind Serde::Deserialize<MetricKind>(const Json&);

template Json Serde::Serialize<BuildConfig>(const BuildConfig&);
template BuildConfig Serde::Deserialize<BuildConfig>(const Json&);

template Json Serde::Serialize<SearchConfig>(const SearchConfig&);
template SearchConfig Serde::Deserialize<SearchConfig>(const Json&);

template Json Serde::Serialize<BenchmarkConfig>(const BenchmarkConfig&);
template BenchmarkConfig Serde::Deserialize<BenchmarkConfig>(const Json&);

template Json Serde::Serialize<RunConfig>(const RunConfig&);
template RunConfig Serde::Deserialize<RunConfig>(const Json&);

template Json Serde::Serialize<DatasetConfig>(const DatasetConfig&);
template DatasetConfig Serde::Deserialize<DatasetConfig>(const Json&);

} // namespace ann

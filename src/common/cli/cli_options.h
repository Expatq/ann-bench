#pragma once

#include <charconv>
#include <filesystem>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <type_traits>
#include <vector>

namespace ann {

class CliOptions {
public:
    CliOptions(int argc, char** argv);

    CliOptions& AddUsage(std::string description);
    CliOptions& AddOption(std::string name, std::string help);
    CliOptions& AddPositional(std::string name, std::string help);
    CliOptions& Required();
    CliOptions& Optional();

    template <typename T>
    CliOptions& StoreResult(T* target) {
        Specs_.back().Assign = [target](std::string_view text) {
            if constexpr (std::is_same_v<T, std::string> ||
                          std::is_same_v<T, std::filesystem::path>) {
                *target = T(text);
            } else {
                static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool>,
                              "StoreResult supports strings, paths and numbers");
                T value{};
                const auto [end, error] =
                    std::from_chars(text.data(), text.data() + text.size(), value);
                if (error != std::errc{} || end != text.data() + text.size()) {
                    throw std::invalid_argument("expected a number in the supported range");
                }
                *target = value;
            }
        };
        return *this;
    }

    // Returns false after printing help. Parsing errors are reported as exceptions.
    bool Parse();

private:
    struct Spec {
        std::string Name;
        std::string Help;
        bool Positional;
        bool Required;
        std::function<void(std::string_view)> Assign;
    };

private:
    static std::string Label(const Spec& spec);

private:
    void PrintHelp() const;

private:
    std::vector<std::string> Tokens_;
    std::vector<Spec> Specs_;
    std::string ProgramName_;
    std::string Description_;
};

} // namespace ann

#pragma once

#include <common/types.h>

#include <filesystem>
#include <optional>

namespace ann {

struct RunArguments {
    BackendKind Backend;
    RunMode Mode;
    std::filesystem::path GeneralConfigPath;
    std::filesystem::path BackendConfigPath;
    std::filesystem::path DatasetPath;
    std::filesystem::path IndexPath;
    std::filesystem::path OutputPath;
};

class BenchmarkCli {
public:
    // Returns nullopt for help. Paths are relative to the working directory.
    // Parsing does not load configurations, datasets or indexes.
    static std::optional<RunArguments> Parse(int argc, char** argv);
};

} // namespace ann

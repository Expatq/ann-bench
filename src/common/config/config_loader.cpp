#include "config_loader.h"

#include <src/serde/serde.h>

#include <fstream>
#include <stdexcept>
#include <string>

namespace ann {

RunConfig ConfigLoader::LoadRun(const std::filesystem::path& path) {
    const auto config = Read(path);
    try {
        return Serde::Deserialize<RunConfig>(config);
    } catch (const std::exception& error) {
        throw std::invalid_argument(path.string() + ": " + error.what());
    }
}

Json ConfigLoader::LoadBackend(const std::filesystem::path& path) {
    return Read(path);
}

Json ConfigLoader::Read(const std::filesystem::path& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("cannot open configuration: " + path.string());
    }
    try {
        auto config = Json::parse(input);
        if (!config.is_object()) {
            throw std::invalid_argument("configuration must be a JSON object");
        }
        return config;
    } catch (const std::exception& error) {
        throw std::invalid_argument(path.string() + ": " + error.what());
    }
}

} // namespace ann

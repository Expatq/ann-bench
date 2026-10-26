#pragma once

#include "config.h"

#include <src/common/types.h>

#include <filesystem>

namespace ann {

class ConfigLoader {
public:
    static RunConfig LoadRun(const std::filesystem::path& path);
    static DatasetConfig LoadDataset(const std::filesystem::path& path);

    // Native options are interpreted by the backend factory.
    static Json LoadBackend(const std::filesystem::path& path);

private:
    static Json Read(const std::filesystem::path& path);
};

} // namespace ann

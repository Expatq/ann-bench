#pragma once

#include "backend.h"
#include "types.h"

#include <memory>

namespace ann {

class IBackendFactory {
public:
    virtual ~IBackendFactory() = default;

    // Parses native options and creates an unbuilt backend that owns its configuration.
    virtual std::unique_ptr<IBackend> Create(const Json& config) const = 0;
};

} // namespace ann

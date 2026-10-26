#pragma once

#include <common/backend_factory.h>

#include <memory>

namespace ann {

class FullScanBackendFactory : public IBackendFactory {
public:
    std::unique_ptr<IBackend> Create(const Json& config) const override;
};

} // namespace ann

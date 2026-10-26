#include "backend.h"
#include "backend_factory.h"

namespace ann {

std::unique_ptr<IBackend> FullScanBackendFactory::Create(const Json&) const {
    return std::make_unique<FullScanBackend>();
}

} // namespace ann

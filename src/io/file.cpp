#include "file.h"

#include <sys/stat.h>

#include <algorithm>
#include <cerrno>
#include <fcntl.h>
#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>
#include <unistd.h>
#include <utility>

namespace ann {

File::File(std::filesystem::path path) : Path_(std::move(path)) {
    Descriptor_ = ::open(Path_.c_str(), O_RDONLY | O_CLOEXEC);
    if (Descriptor_ < 0) {
        ThrowError("open");
    }
}

File::~File() {
    ::close(Descriptor_);
}

std::size_t File::Size() const {
    struct stat info;
    if (::fstat(Descriptor_, &info) != 0) {
        ThrowError("stat");
    }
    return static_cast<std::size_t>(info.st_size);
}

void File::Read(std::size_t offset, std::span<std::uint8_t> destination) const {
    while (!destination.empty()) {
        const auto count = std::min(destination.size(),
                                    static_cast<std::size_t>(std::numeric_limits<ssize_t>::max()));
        const auto bytes =
            ::pread(Descriptor_, destination.data(), count, static_cast<off_t>(offset));
        if (bytes < 0) {
            if (errno == EINTR) {
                continue;
            }
            ThrowError("read");
        }
        if (bytes == 0) {
            throw std::runtime_error("unexpected end of file '" + Path_.string() + "' at byte " +
                                     std::to_string(offset));
        }
        offset += static_cast<std::size_t>(bytes);
        destination = destination.subspan(static_cast<std::size_t>(bytes));
    }
}

void File::ThrowError(const char* operation) const {
    const auto error = errno;
    throw std::system_error(error, std::generic_category(),
                            std::string(operation) + " '" + Path_.string() + "'");
}

} // namespace ann

#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>

namespace ann {

class IFile {
public:
    virtual ~IFile() = default;

    virtual std::size_t Size() const = 0;

    virtual void Read(std::size_t offset, std::span<std::uint8_t> destination) const = 0;
};

class File : public IFile {
public:
    explicit File(std::filesystem::path path);
    ~File() override;

    File(const File&) = delete;
    File& operator=(const File&) = delete;

    std::size_t Size() const override;
    void Read(std::size_t offset, std::span<std::uint8_t> destination) const override;

private:
    [[noreturn]] void ThrowError(const char* operation) const;

private:
    std::filesystem::path Path_;
    int Descriptor_;
};

} // namespace ann

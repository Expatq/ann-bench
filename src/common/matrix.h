#pragma once

#include <cstddef>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

namespace ann {

template <typename T = float>
struct MatrixView {
    const T* Data;
    std::size_t Rows;
    std::size_t Dim;

    std::span<const T> Row(std::size_t row) const {
        return std::span(Data, Rows * Dim).subspan(row * Dim, Dim);
    }

    MatrixView Slice(std::size_t begin, std::size_t count) const {
        return {Data ? Data + begin * Dim : nullptr, count, Dim};
    }
};

template <typename T = float>
class Matrix {
public:
    Matrix(std::size_t rows, std::size_t dim, const T& value = T{})
        : Data_(rows * dim, value), Rows_(rows), Dim_(dim) {
        if (dim == 0) {
            throw std::invalid_argument("matrix dimension must be positive");
        }
    }

    Matrix(const Matrix&) = delete;
    Matrix& operator=(const Matrix&) = delete;

    Matrix(Matrix&& other) noexcept
        : Data_(std::exchange(other.Data_, {})), Rows_(std::exchange(other.Rows_, 0)),
          Dim_(other.Dim_) {}

    Matrix& operator=(Matrix&& other) noexcept {
        if (this != &other) {
            Data_ = std::exchange(other.Data_, {});
            Rows_ = std::exchange(other.Rows_, 0);
            Dim_ = other.Dim_;
        }
        return *this;
    }

    MatrixView<T> View() const {
        return {Data_.data(), Rows_, Dim_};
    }

    std::span<T> Data() {
        return Data_;
    }

    std::span<const T> Data() const {
        return Data_;
    }

    std::span<T> Row(std::size_t row) {
        return Data().subspan(row * Dim_, Dim_);
    }

    std::span<const T> Row(std::size_t row) const {
        return Data().subspan(row * Dim_, Dim_);
    }

    void Resize(std::size_t rows, std::size_t dim, const T& value = T{}) {
        if (dim == 0) {
            throw std::invalid_argument("matrix dimension must be positive");
        }
        Data_.assign(rows * dim, value);
        Rows_ = rows;
        Dim_ = dim;
    }

    std::vector<T> TakeData() && {
        Rows_ = 0;
        return std::exchange(Data_, {});
    }

private:
    std::vector<T> Data_;
    std::size_t Rows_;
    std::size_t Dim_;
};

} // namespace ann

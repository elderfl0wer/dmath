#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <concepts>
#include <cstddef>

#include "../vector/vector.hpp"

namespace dmath {

template<std::floating_point T, std::size_t Rows, std::size_t Columns>
class matrix {
public:
    vec<T, Columns> data[Rows];

    const T& operator()(std::size_t row, std::size_t col) const {
        return data[row-1](col);
    }
    T& operator()(std::size_t row, std::size_t col) {
        return data[row-1](col);
    }
    constexpr bool operator==(const matrix<T, Rows, Columns>& other) const {
        for (size_t i = 1; i <= Rows; i++) {
            for (size_t j = 1; j <= Columns; j++) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }
        return true;
    }
    constexpr auto operator<=>(const matrix<T, Rows, Columns>& other) const = default;


    constexpr void fill(const T k) {
        for (size_t i = 1; i <= Rows; i++) {
            for (size_t j = 1; j <= Columns; j++) {
                (*this)(i, j) = k;
            }
        }
        return;
    }
    constexpr void scale(const T k) {
        for (size_t i = 1; i <= Rows; i++) {
            for (size_t j = 1; j <= Columns; j++) {
                (*this)(i, j) *= k;
            }
        }
        return;
    }
};
}

#endif

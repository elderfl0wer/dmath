#include <array>
#include <concepts>
#include <cstddef>
#include <stdbool.h>

namespace dmath {

template<std::floating_point T, std::size_t Rows, std::size_t Cols>
class matrix {
public:
    std::array<std::array<T, Cols>, Rows> data{};

    T& operator()(std::size_t row, std::size_t col) {
        return data[row][col];
    }

    const T& operator()(std::size_t row, std::size_t col) const {
        return data[row][col];
    }
    constexpr bool operator==(matrix<T, Rows, Cols>& other) const {
        for (size_t i = 0; i <= Rows; i++) {
            for (size_t j = 0; j <= Cols; j++) {
                if ((*this)(i, j) != other(i, j)) {
                    return false;
                }
            }
        }

        return true;
    }
    constexpr matrix<T, Rows, Cols> operator+(matrix<T, Rows, Cols>& other) {
        matrix<T, Rows, Cols> ans;

        for (size_t i = 0; i <= Rows; i++) {
            for (size_t j = 0; j <= Cols; j++) {
                ans(i, j) = (*this)(i, j) + other(i, j);
            }
        }
        return ans;
    }


    constexpr void fill(const T k) const {
        for (size_t i = 0; i <= Rows; i++) {
            for (size_t j = 0; j <= Cols; j++) {
                (*this)(i, j) = k;
            }
        }

        return;
    }
    constexpr void scale(const T k) const {
        for (size_t i = 0; i <= Rows; i++) {
            for (size_t j = 0; j <= Cols; j++) {
                (*this)(i, j) *= k;
            }
        }

        return;
    }

};
}

#ifndef MATRIX2_CPP
#define MATRIX2_CPP

#include <cmath>
#include <concepts>
#include <cstddef>
#include <stdexcept>

#include "../vector/vector2.hpp"

template<std::floating_point T>
class mat2 {
public:
    vec2<T> rows[2]; // the horizontal ones

    constexpr const vec2<T>& operator[](std::size_t i) const {
        return rows[i];
    }
    constexpr T& operator()(std::size_t row, std::size_t col) {
        if (row < 1 || row > 2 || col < 1 || col > 2) {
        throw std::out_of_range("index out of range");
        }

        return col == 1
            ? rows[row-1].x
            : rows[row-1].y;
    }
    constexpr mat2<T>& operator+(const T k) const {
        mat2<T> ans = *this;
        ans(1, 1) += k;
        ans(1, 2) += k;
        ans(2, 1) += k;
        ans(2, 2) += k;

        return ans;
    }
    constexpr mat2<T>& operator+(const mat2<T>& other) {
        mat2<T> ans;
        ans(1, 1) = (*this)(1, 1) + other(1, 1);
        ans(1, 2) = (*this)(1, 2) + other(1, 2);
        ans(2, 1) = (*this)(2, 1) + other(2, 1);
        ans(2, 2) = (*this)(2, 2) + other(2, 2);

        return ans;
    }
    constexpr mat2<T>& operator-(const mat2<T>& other) {
        mat2<T> ans;
        ans(1, 1) = (*this)(1, 1) - other(1, 1);
        ans(1, 2) = (*this)(1, 2) - other(1, 2);
        ans(2, 1) = (*this)(2, 1) - other(2, 1);
        ans(2, 2) = (*this)(2, 2) - other(2, 2);

        return ans;
    }
    constexpr mat2<T>& operator*(const T k) const {
        mat2<T> ans = *this;
        ans(1, 1) *= k;
        ans(1, 2) *= k;
        ans(2, 1) *= k;
        ans(2, 2) *= k;

        return ans;
    }
    constexpr mat2<T>& operator*(const mat2<T>& other) {
        mat2<T> ans;
        ans(1, 1) = (*this)(1, 1)*other(1, 1) + (*this)(1, 2)*other(2, 1);
        ans(1, 2) = (*this)(1, 1)*other(1, 2) + (*this)(1, 2)*other(2, 2);
        ans(2, 1) = (*this)(2, 1)*other(1, 1) + (*this)(2, 2)*other(2, 1);
        ans(2, 2) = (*this)(2, 1)*other(1, 2) + (*this)(2, 2)*other(2, 2);

        return ans;
    }



    constexpr T magnitude() {
        rows[0].x*rows[1].y - rows[0].y*rows[1].x;
    }
    bool is_singular() {
        if (magnitude() == 0) {
            return true;
        }
        return false;
    }
    void transpose() {
        mat2<T> k = *this;

        rows[1].x = k(1, 2);
        rows[0].y = k(2, 1);
    }

};

// using mat2i = mat2<int>;
using mat2f = mat2<float>;
using mat2d = mat2<double>;

template<std::floating_point T>
mat2<T> minor(mat2<T>& m);
template<std::floating_point T>
mat2<T> cofactor(mat2<T>& m);
template<std::floating_point T>
mat2<T> adjacent(mat2<T>& m);
template<std::floating_point T>
mat2<T> inverse(mat2<T>& m);

#endif /* MATRIX2_CPP */


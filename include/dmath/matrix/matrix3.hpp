#ifndef MATRIX3_HPP
#define MATRIX3_HPP

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

#include "../vector/vector3.hpp"

template<std::floating_point T>
class mat3 {
public:
    vec3<T> rows[3]; // the horizontal ones
    
    constexpr const vec3<T>& operator[](std::size_t i) const {
        return rows[i];
    }
    constexpr T& operator()(std::size_t row, std::size_t col) {
        if (row < 1 || row > 3 || col < 1 || col > 3) {
        throw std::out_of_range("index out of range");
        }
        switch (col) {
            case 1: return rows[row-1].x;
            case 2: return rows[row-1].y;
            case 3: return rows[row-1].z;
        }       
    }
    constexpr mat3<T> operator+(const T k) const {
        mat3<T> ans = *this;
        ans(1, 1) += k; ans(1, 2) += k; ans(1, 3) += k;
        ans(2, 1) += k; ans(2, 2) += k; ans(2, 3) += k;
        ans(3, 1) += k; ans(3, 2) += k; ans(3, 3) += k;

        return ans;
    }
    constexpr mat3<T> operator+(const mat3<T>& other) const {
        mat3<T> ans = *this;
        ans(1, 1) += other(1, 1); ans(1, 2) += other(1, 2); ans(1, 3) += other(1, 3);
        ans(2, 1) += other(2, 1); ans(2, 2) += other(2, 2); ans(2, 3) += other(2, 3);
        ans(3, 1) += other(3, 1); ans(3, 2) += other(3, 2); ans(3, 3) += other(3, 3);

        return ans;
    }
    constexpr mat3<T> operator-(const mat3<T>& other) const {
        mat3<T> ans = *this;
        ans(1, 1) -= other(1, 1); ans(1, 2) -= other(1, 2); ans(1, 3) -= other(1, 3);
        ans(2, 1) -= other(2, 1); ans(2, 2) -= other(2, 2); ans(2, 3) -= other(2, 3);
        ans(3, 1) -= other(3, 1); ans(3, 2) -= other(3, 2); ans(3, 3) -= other(3, 3);

        return ans;
    }
    constexpr mat3<T> operator-(const T k) const {
        mat3<T> ans = *this;
        ans(1, 1) -= k; ans(1, 2) -= k; ans(1, 3) -= k;
        ans(2, 1) -= k; ans(2, 2) -= k; ans(2, 3) -= k;
        ans(3, 1) -= k; ans(3, 2) -= k; ans(3, 3) -= k;

        return ans;
    }
    constexpr mat3<T> operator*(const T k) const {
        mat3<T> ans = *this;
        ans(1, 1) *= k; ans(1, 2) *= k; ans(1, 3) *= k;
        ans(2, 1) *= k; ans(2, 2) *= k; ans(2, 3) *= k;
        ans(3, 1) *= k; ans(3, 2) *= k; ans(3, 3) *= k;

        return ans;
    }

    static constexpr mat3<T> identity() {
        return {
            {1, 0, 0},
            {0 ,1, 0},
            {0, 0, 1}
        };
    }
    static constexpr mat3<T> zero() {
        return {
            {0, 0, 0},
            {0 ,0, 0},
            {0, 0, 0}
        };
    }
    
    constexpr T magnitude() const {
        mat3<T> ans = *this;
     
        return ans(1, 1)*(ans(2, 2)*ans(3, 3)-ans(2, 3)*ans(3, 2)) - ans(1, 2)*(ans(2, 1)*ans(3, 3)-ans(2, 3)*ans(3, 1) + ans(1, 3)*(ans(2, 1)*ans(3, 2)- ans(2, 2)*ans(3, 1)));
    }
    constexpr bool is_singular() const {
        if ((*this).magnitude() == 0) {
            return true;
        }

        return false;
    }
    constexpr T trace() const {
        mat3<T> k = *this;

        return k(1, 1)+k(2, 2)+k(3, 3);
    }
};

#endif /* MATRIX3_HPP */

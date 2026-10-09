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
    constexpr mat3<T> operator=(const mat3<T>& other) const {
        rows[0] = other.rows[0];
        rows[1] = other.rows[1];
        rows[2] = other.rows[2];

        return *this;
    }
    constexpr mat3<T> operator*(const T k) const {
        mat3<T> ans = *this;
        ans(1, 1) *= k; ans(1, 2) *= k; ans(1, 3) *= k;
        ans(2, 1) *= k; ans(2, 2) *= k; ans(2, 3) *= k;
        ans(3, 1) *= k; ans(3, 2) *= k; ans(3, 3) *= k;

        return ans;
    }
    constexpr bool operator==(const mat3<T>& other) const {
        mat3<T> m = *this;

        if (
            m(1, 1) == other(1, 1) && m(1, 2) == other(1, 2) && m(1, 3) == other(1, 3) &&
            m(2, 1) == other(2, 1) && m(2, 2) == other(2, 2) && m(2, 3) == other(2, 3) &&
            m(3, 1) == other(3, 1) && m(3, 2) == other(3, 2) && m(3, 3) == other(3, 3)
        ) {

        return true;
    }

        return false;
    }
    constexpr bool operator!=(const mat3<T>& other) const {
        mat3<T> m = *this;

        if (
            m(1, 1) == other(1, 1) && m(1, 2) == other(1, 2) && m(1, 3) == other(1, 3) &&
            m(2, 1) == other(2, 1) && m(2, 2) == other(2, 2) && m(2, 3) == other(2, 3) &&
            m(3, 1) == other(3, 1) && m(3, 2) == other(3, 2) && m(3, 3) == other(3, 3)
        ) {

        return false;
    }

        return true;
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
    constexpr void fill(const T k) const {
        (*this)(1, 1) = k;
        (*this)(1, 2) = k;
        (*this)(1, 3) = k;
        (*this)(2, 1) = k;
        (*this)(2, 2) = k;
        (*this)(2, 3) = k;
        (*this)(3, 1) = k;
        (*this)(3, 2) = k;
        (*this)(3, 3) = k;

        return;
    }
    constexpr mat3<T> transpose() const {
        mat3<T> k = *this;

        k.rows[0].y = (*this)(2, 1);
        k.rows[0].z = (*this)(3, 1);

        k.rows[1].x = (*this)(1, 2);
        k.rows[1].z = (*this)(3, 2);

        k.rows[2].x = (*this)(1, 3);
        k.rows[2].y = (*this)(2, 3);

        return k;
    }
    constexpr bool is_symmetric() const {
        mat3<T> m = *this;
        if (m.transpose() == (*this)) {
            return true;
        }

        return false;
    }
    constexpr bool is_skew_symmetric() const {
        mat3<T> m = *this;
        if (-1*m.transpose() == m) {
            return true;
        }
        
        return false;
    }

};

#endif /* MATRIX3_HPP */

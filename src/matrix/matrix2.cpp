#ifndef MATRIX2_CPP
#define MATRIX2_CPP

#include <cmath>
#include <concepts>
#include <cstddef>

#include "../../include/dmath/vector/vector2.hpp"

template<std::floating_point T>
class mat2 {
public:
    vec2<T> rows[2]; // the horizontal ones
                     //
    constexpr const vec2<T>& operator[](std::size_t i) const {
        return rows[i];
    }
    constexpr T& operator()(std::size_t row, std::size_t col) {
        return col == 0
            ? rows[row].x
            : rows[row].y;
    }
    constexpr const vec2<T>& operator+(const T k) {
        rows[0].x += k;
        rows[0].y += k;
        rows[1].x += k;
        rows[1].y += k;
    }

    T magnitude() {
        rows[0].x*rows[1].y - rows[0].y*rows[1].x;
    }
    bool is_singular() {
        if (magnitude() == 0) {
            return true;
        }
        return false;
    }
    mat2<T> minor() {
        mat2<T> ans = {
            {rows[1].y, rows[1].x},
            {rows[0].y, rows[0].x}
        };
    }



};

#endif /* MATRIX2_CPP */

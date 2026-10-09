#include <cmath>
#include <concepts>
#include <stdexcept>

#include "../../include/dmath/matrix/matrix3.hpp"
#include "../../include/dmath/matrix/matrix2.hpp"


template<std::floating_point T>
T minor(const mat3<T>& m, std::size_t row, std::size_t col)
{
    mat2<T> sub;

    int r = 1;
    for (std::size_t i = 1; i <= 3; i++) {
        if (i == row)
            continue;

        int c = 1;
        for (std::size_t j = 1; j <= 3; j++) {
            if (j == col)
                continue;

            sub(r, c) = m(i, j);
            c++;
        }
        r++;
    }

    return sub.magnitude();
}


template<std::floating_point T>
mat3<T> cofactor(const mat3<T>& m)
{
    mat3<T> ans;

    for (std::size_t i = 1; i <= 3; i++) {
        for (std::size_t j = 1; j <= 3; j++) {

            ans(i, j) = minor(m, i, j);

            // checkerboard sign pattern
            if ((i + j) % 2 == 1)
                ans(i, j) *= -1;
        }
    }

    return ans;
}


template<std::floating_point T>
mat3<T> adjacent(const mat3<T>& m)
{
    return cofactor(m).transpose();
}


template<std::floating_point T>
mat3<T> inverse(const mat3<T>& m)
{
    if (m.is_singular()) {
        throw std::out_of_range("matrix is uninversable");
    }

    return adjacent(m) / m.magnitude();
}

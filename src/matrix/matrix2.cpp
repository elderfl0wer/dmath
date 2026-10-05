#include <cmath>
#include <concepts>
#include <stdexcept>

#include "../../include/dmath/matrix/matrix2.hpp"

template<std::floating_point T>
mat2<T> minor(mat2<T>& m) 
{
    mat2<T> ans;
    ans(1, 1) = m(2, 2);
    ans(1, 2) = m(2, 1);
    ans(2, 1) = m(1, 2);
    ans(2, 2) = m(1, 1);

    return ans;
}

template<std::floating_point T>
mat2<T> cofactor(mat2<T>& m)
{
    mat2<T> ans = minor(m);
    ans(1, 2) *= -1;
    ans(2, 1) *= -1;

    return ans;
}

template<std::floating_point T>
mat2<T> adjacent(mat2<T>& m)
{
    mat2<T> ans = cofactor(m);
    
    return ans.transpose();
}

template<std::floating_point T>
mat2<T> inverse(mat2<T>& m)
{
    if (m.is_singular()) {
        throw std::out_of_range("matrix is uninversable");
    }

    return m.adjacent / m.magnitude();
}

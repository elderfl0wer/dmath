#include <cmath>
#include <concepts>

#include "../../include/dmath/matrix/matrix2.hpp"

template<std::floating_point T>
mat2<T> minor(mat2<T>& m) 
{
        mat2<T> ans;
        ans(1, 1) = m[1].y;
        ans(1, 2) = m[1].x;
        ans(2, 1) = m[0].y;
        ans(2, 2) = m[0].x;

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


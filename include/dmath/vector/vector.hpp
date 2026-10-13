#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>

namespace dmath {

template<std::floating_point T, std::size_t N>
class vector {
public:
    std::array<T, N> data;
};
}

#endif /* VECTOR_HPP */

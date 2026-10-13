#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <stdexcept>

namespace dmath {

template<std::floating_point T, std::size_t N>
class vec {
public:
    std::array<T, N> data;
    // vec<double, 4> v = {1, 2, 3, 4};

    constexpr T& operator()(std::size_t i) {
        if (i < 1 || i > N)
            throw std::out_of_range("vector index out of range");

        return data[i - 1];
    }

    constexpr const T& operator()(std::size_t i) const {
        if (i < 1 || i > N)
            throw std::out_of_range("vector index out of range");

        return data[i - 1];
    }
    constexpr bool operator==(const vec&) const = default;
    // C++20 generates `!=` operator automatically inferred from `==`
};
}

#endif /* VECTOR_HPP */

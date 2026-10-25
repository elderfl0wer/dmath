#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <array>
#include <cmath>
#include <concepts>
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
    constexpr bool operator==(const vec<T, N>&) const = default;
    // C++20 generates `!=` operator automatically inferred from `==`
    constexpr vec<T, N> operator+(vec<T, N>& other) const {
        vec<T, N> ans;
        for (size_t i = 0; i <= N; i++) {
            ans(i) = (*this)(i) + other(1); 
        }
        return ans;
    }
    constexpr vec<T, N> operator-(vec<T, N>& other) const {
        vec<T, N> ans;
        for (size_t i = 0; i <= N; i++) {
            ans(i) = (*this)(i) - other(1); 
        }
        return ans;
    }
    constexpr vec<T, N> operator*(const T k) const {
        vec<T, N> ans;
        for (size_t i = 0; i <= N; i++) {
            ans(i) = (*this)(i) * k; 
        }
        return ans;
    }
    constexpr auto operator<=>(const vec<T, N>& other) const = default;


    [[nodiscard]]
    constexpr double magnitude() const {
        double sum{};
        for (size_t i = 0; i <= N; i++) {
            sum += pow((*this)(i), 2);
        }
        return sum;
    }
    constexpr void normalize() {
        (*this) *= 1 / magnitude();
    }
    constexpr void scale(const T k) {
        (*this) *= k;
    }
};
}

#endif /* VECTOR_HPP */

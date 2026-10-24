// descriptive.hpp - basic descriptive statistics over ranges of floating-point values.
//
// Conventions:
//   * Empty input: mean, median, mode, variance, stddev, maximum, minimum and
//     distribution_range throw std::invalid_argument. sum() of an empty range is 0
//     and product() is 1.
//   * NaN values are not supported (ordering-based functions would be ill-defined).
//   * variance() is the POPULATION variance (divides by n); stddev() is its square root.
//   * mode() returns the most frequent value; ties resolve to the smallest value.
//   * distribution_range() is maximum - minimum.
#include <algorithm>
#include <concepts>
#include <cstddef>
#include <limits>
#include <ranges>
#include <stdexcept>
#include <cmath>
#include <vector>

namespace detail {

[[noreturn]] constexpr void empty_error(const char* fn) {
    throw std::invalid_argument(fn);  // message: name of the function that got an empty range
}

// Newton iteration so stddev can be evaluated at compile time (std::sqrt is not
// constexpr until C++26). At runtime we defer to std::sqrt.
template <std::floating_point T>
constexpr T sqrt_impl(T x) {
    if consteval {
        if (x != x || x < T{0}) return std::numeric_limits<T>::quiet_NaN();
        if (x == T{0} || x == std::numeric_limits<T>::infinity()) return x;
        T g = x >= T{1} ? x : T{1};
        for (int i = 0; i < 4096; ++i) {
            const T next = T{0.5} * (g + x / g);
            if (next == g) break;
            g = next;
        }
        return g;
    } else {
        return std::sqrt(x);
    }
}

template <class R>
constexpr auto to_vector(const R& r) {
    using T = std::ranges::range_value_t<R>;
    std::vector<T> v;
    if constexpr (std::ranges::sized_range<const R>) {
        v.reserve(static_cast<std::size_t>(std::ranges::size(r)));
    }
    for (const auto& x : r) v.push_back(x);
    return v;
}

}  // namespace detail

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto mean(const R& r) {
    using T = std::ranges::range_value_t<R>;
    T total{};
    std::size_t n = 0;
    for (const auto& x : r) {
        total += x;
        ++n;
    }
    if (n == 0) detail::empty_error("mean: empty range");
    return total / static_cast<T>(n);
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto median(const R& r) {
    using T = std::ranges::range_value_t<R>;
    auto v = detail::to_vector(r);
    if (v.empty()) detail::empty_error("median: empty range");

    const std::size_t n = v.size();
    const auto mid = v.begin() + static_cast<std::ptrdiff_t>(n / 2);
    std::nth_element(v.begin(), mid, v.end());
    const T upper = *mid;
    if (n % 2 == 1) return upper;

    const T lower = *std::max_element(v.begin(), mid);
    return lower + (upper - lower) / T{2};  // avoids overflow of (a + b)
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto mode(const R& r) {
    using T = std::ranges::range_value_t<R>;
    auto v = detail::to_vector(r);
    if (v.empty()) detail::empty_error("mode: empty range");

    std::ranges::sort(v);
    T best = v.front();
    std::size_t best_count = 0;
    const std::size_t n = v.size();
    for (std::size_t i = 0; i < n;) {
        std::size_t j = i + 1;
        while (j < n && v[j] == v[i]) ++j;
        if (j - i > best_count) {  // strict '>' keeps the smallest value on ties
            best_count = j - i;
            best = v[i];
        }
        i = j;
    }
    return best;
}

// Population variance, computed in a single pass with Welford's algorithm.
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto variance(const R& r) {
    using T = std::ranges::range_value_t<R>;
    T running_mean{};
    T m2{};
    std::size_t n = 0;
    for (const auto& x : r) {
        ++n;
        const T delta = x - running_mean;
        running_mean += delta / static_cast<T>(n);
        m2 += delta * (x - running_mean);
    }
    if (n == 0) detail::empty_error("variance: empty range");
    return m2 / static_cast<T>(n);
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto stddev(const R& r) {
    return detail::sqrt_impl(variance(r));
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto maximum(const R& r) {
    auto it = std::ranges::begin(r);
    const auto end = std::ranges::end(r);
    if (it == end) detail::empty_error("maximum: empty range");
    auto best = *it;
    for (++it; it != end; ++it) {
        if (*it > best) best = *it;
    }
    return best;
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto minimum(const R& r) {
    auto it = std::ranges::begin(r);
    const auto end = std::ranges::end(r);
    if (it == end) detail::empty_error("minimum: empty range");
    auto best = *it;
    for (++it; it != end; ++it) {
        if (*it < best) best = *it;
    }
    return best;
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto sum(const R& r) {
    using T = std::ranges::range_value_t<R>;
    T total{};
    for (const auto& x : r) total += x;
    return total;
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto product(const R& r) {
    using T = std::ranges::range_value_t<R>;
    T result{1};
    for (const auto& x : r) result *= x;
    return result;
}

// maximum - minimum, in a single pass.
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto distribution_range(const R& r) {
    auto it = std::ranges::begin(r);
    const auto end = std::ranges::end(r);
    if (it == end) detail::empty_error("distribution_range: empty range");
    auto lo = *it;
    auto hi = lo;
    for (++it; it != end; ++it) {
        if (*it < lo) lo = *it;
        if (*it > hi) hi = *it;
    }
    return hi - lo;
}
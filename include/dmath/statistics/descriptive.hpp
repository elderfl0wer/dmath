#ifndef DESCRIPTIVE_HPP
#define DESCRIPTIVE_HPP

#include <concepts>
#include <ranges>

namespace dmath {

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto mean(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto median(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto mode(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto variance(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto stddev(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto maximum(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto minimum(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto sum(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto product(const R& r);
template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto distribution_range(const R& r);

};

#endif /* DESCRIPTIVE_HPP */

#include <concepts>
#include <cmath>
#include <ranges>
#include <algorithm>

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto mean(const R& r)
{
	auto len = std::ranges::size(r);
	auto it = std::ranges::begin(r);

	auto inc = it;

	auto sum = 0;
	
	for (auto i = 0; i <= len; i++) {
		sum += *inc;
	
		inc++;
	}

	return sum / len;
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto median(const R& r)
{
    auto k = r;
    std::ranges::sort(k);

	auto len = std::ranges::size(k);
	auto it = std::ranges::begin(k);

	auto inc = it;

    if (len % 2 != 0) {
        return *((inc/2)+1);
    } else {
        return ( *(inc / 2) + *((inc+2) / 2) ) / 2;
    }
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto mode(const R& r)
{
    return 3*median(r) - 2*mean(r);
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto variance(const R& r)
{
    auto k = r;

	auto len = std::ranges::size(k);
	auto it = std::ranges::begin(k);

    auto kmean = mean(k);

	auto inc = it;
    
    auto sum = 0;

    for (auto i = 0; i <= len; i++) {
        sum += pow( *(inc+i) - kmean , 2);
    }

    return sum / len;
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto stddev(const R& r)
{
    return sqrt(variance(r));
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto maximum(const R& r)
{
    auto k = r;
    std::ranges::sort(k);

    return *(std::ranges::prev(std::ranges::end(k)));
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto minimum(const R& r)
{
    auto k = r;
    std::ranges::sort(k);

    return *(std::ranges::begin(r));
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto sum(const R& r)
{
	auto len = std::ranges::size(r);
	auto it = std::ranges::begin(r);

	auto inc = it;

	auto sum = 0;
	
	for (auto i = 0; i <= len; i++) {
		sum += *inc;
	
		inc++;
	}

	return sum;
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto product(const R& r)
{
	auto len = std::ranges::size(r);
	auto it = std::ranges::begin(r);

	auto inc = it;

	auto product = 0;
	
	for (auto i = 0; i <= len; i++) {
		product *= *inc;
	
		inc++;
	}

	return product;
}

template <std::ranges::input_range R>
requires std::floating_point<std::ranges::range_value_t<R>>
constexpr auto distribution_range(const R& r)
{
    return maximum(r) - minimum(r);
}

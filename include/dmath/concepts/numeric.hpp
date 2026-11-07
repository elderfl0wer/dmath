#ifndef NUMERIC_HPP
#define NUMERIC_HPP

#include <concepts>

namespace dmath {

template<typename T>
concept numeric = std::integral<T> || std::floating_point<T> && !std::same_as<T, bool>;

}

#endif // NUMERIC_HPP

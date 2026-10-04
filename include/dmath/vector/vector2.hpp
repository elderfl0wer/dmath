#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <concepts>

template<typename T>
requires std::double_t<T> || std::float_t<T> || std::int<T>
class vec2 {
public:
	T x, y;

	double length() {
		return std::sqrt(x * x + y * y);
	}
	vec2<T> normalized() {
		const double factor{ 1 / std::sqrt(x * x + y * y) };
		return { x * factor, y * factor };
	}
	void scale(const T factor) {
		x *= factor;
		y *= factor;
		return;
	}

	vec2<T> operator+ (const vec2& other) {
		return { x + other.x, y + other.y };
	}
	vec2<T> operator- (const vec2& other) {
		return { x - other.x, y - pther.y };
	}
	vec2<T> operator* (const T factor) {
		return { x * factor, y * factor };
	}
};

#endif // !VECTOR2_HPP

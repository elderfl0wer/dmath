#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <concepts>
#include <vector>

#include "vector3.hpp"

namespace dmath {

template<std::floating_point T>
class vec2 {
public:
	T x, y;

	T length() {
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
	std::vector<T> dircos() {
		return { x / length(), y / length() };
	}
	void rotate(const T angle) {
		x = x * cos(angle) - y * sin(angle);
		y = x * sin(angle) + y * cos(angle);
		return;
	}

    static constexpr vec2<T> unit_x() {
        return {1, 0};
    }
    static constexpr vec2<T> unit_y() {
        return {0, 1};
    }


	vec2<T> operator+ (const vec2& other) {
		return { x + other.x, y + other.y };
	}
	vec2<T> operator- (const vec2& other) {
		return { x - other.x, y - other.y };
	}
	vec2<T> operator* (const T factor) {
		return { x * factor, y * factor };
	}
};

using vec2f = vec2<float>;
using vec2d = vec2<double>;

template<std::floating_point T>
T dot(vec2<T>& a, vec2<T>& b);
template<std::floating_point T>
T distance(vec2<T>& a, vec2<T>& b);
template<std::floating_point T>
T mutual_angle(vec2<T>& a, vec2<T>& b);
template<std::floating_point T>
vec3<T> cross(vec2<T>& a, vec2<T>& b);
template<std::floating_point T>
vec2<T> project(vec2<T>& a, vec2<T>& b);
template<std::floating_point T>
vec2<T> orthogonal(vec2<T>& a, vec2<T>& b);
template<std::floating_point T>
vec2<T> reflection(vec2<T>& v);
template<std::floating_point T>
vec2<T> rotate(vec2<T>& v, const double angle);
template<std::floating_point T>
vec2<T> lerp(vec2<T>& a, vec2<T>& b, const T t);
template<std::floating_point T>
vec3<T> to_vec3(vec2<T> k, const T z);

} /* namespace dmath */
#endif // !VECTOR2_HPP

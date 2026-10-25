#ifndef VECTOR2_HPP
#define VECTOR2_HPP

#include <cmath>
#include <concepts>

#include "vector3.hpp"

namespace dmath {

template<std::floating_point T>
class vec2 {
public:
	T x, y;

	constexpr T length() const {
		return std::sqrt(x * x + y * y);
	}
	constexpr vec2<T> normalized() {
		const T factor{ 1 / std::sqrt(x * x + y * y) };
		return { x * factor, y * factor };
	}
	constexpr void scale(const T factor) {
		x *= factor;
		y *= factor;
		return;
	}
	constexpr std::array<T, 2> dircos() const {
		return { x / length(), y / length() };
	}
	constexpr void rotate(const T angle) {
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


	constexpr vec2<T> operator+(const vec2& other) const {
		return { x + other.x, y + other.y };
	}
	constexpr vec2<T> operator-(const vec2& other) const {
		return { x - other.x, y - other.y };
	}
	constexpr vec2<T> operator* (const T factor) const {
		return { x * factor, y * factor };
	}
	constexpr bool operator==(const vec2& other) const {
		if ((*this).x == other.x && (*this).y == other.y) {
			return true;
		}
		return false;
	}
	constexpr auto operator<=>(const vec2& other) const = default;
};

using vec2f = vec2<float>;
using vec2d = vec2<double>;

using namespace dmath;

template<std::floating_point T>
T dot(vec2<T>& a, vec2<T>& b)
{
	return a->x * b->x + a->y * b->y;
}

template<std::floating_point T>
T distance(vec2<T>& a, vec2<T>& b)
{
	vec2<T> ans = a - b;
	return ans.length();
}

template<std::floating_point T>
T mutual_angle(vec2<T>& a, vec2<T>& b)
{
	return acos(dot<T>(a, b) / (a->length() * b->length()));
}

template<std::floating_point T>
vec3<T> cross(vec2<T>& a, vec2<T>& b)
{
	return { 0, 0, a->xb->y - a->y * b->x };
}

template<std::floating_point T>
vec2<T> project(vec2<T>& a, vec2<T>& b)
{
	T k1 = dot<T>(a, b);
	T k2 = std::pow(b->length(), 2);

	return b * (k1 / k2);
}

template<std::floating_point T>
vec2<T> orthogonal(vec2<T>& a, vec2<T>& b)
{
	vec2<T> k = project<T>(a, b);
	return a - k;
}

template<std::floating_point T>
vec2<T> reflection(vec2<T>& v)
{
	vec2<T> n = v->normalized;
	
	return v - (2 * (dot<T>(v, n))) * n;
}

template<std::floating_point T>
vec2<T> rotate(vec2<T>& v, const T angle)
{
	return { v->x * cos(angle) - v->y * sin(angle), v->x * sin(angle) + v->y * cos(angle) };
}

template<std::floating_point T>
vec2<T> lerp(vec2<T>& a, vec2<T>& b, const T t)
{
    return a + t*(b-a);
}


} /* namespace dmath */
#endif // !VECTOR2_HPP
#include <cmath>
#include <concepts>

#include "../../include/dmath/dmath.hpp"

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

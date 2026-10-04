#include <cmath>
#include <concepts>

#include "../../include/dmath/dmath.hpp"

template<std::floating_point T>
T dot(vec3<T>& a, vec3<T>& b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

template<std::floating_point T>
vec3<T> cross(vec3<T>& a, vec3<T>& b)
{
	return { a->y * b->z - b->y * a->z, b->x * a->z - a->x * b->z, a->x * b->y - b->x * a->y };
}

template<std::floating_point T>
vec3<T> rotate(vec3<T>& v, const vec3<T>& other, const T angle) 
{
	vec3<T> k = other.normalized();

	vec3<T> k1 = v * cos(angle);
	vec3<T> k2 = cross<T>(k, v) * sin(angle);
	vec3<T> k3 = (dot<T>(k, v) * (1 - cos(angle))) * k;

	return k1 + k2 + k3;
}

template<std::floating_point T>
T distance(vec3<T>& a, vec3<T>& b)
{
    vec3<T> ans = a-b;
    return ans.length();
}

template<std::floating_point T>
T mutual_angle(vec3<T>& a, vec3<T>& b)
{
    return std::acos(dot<T>(a, b) / (a->length()*b->length()));
}

template<std::floating_point T>
vec3<T> project(vec3<T>& a, vec3<T>& b)
{
    T k = dot<T>(a, b) / std::pow(b->length(), 2);

    return k * b;
}

template<std::floating_point T>
vec3<T> orthogonal(vec3<T>& a, vec3<T>& b)
{
	vec3<T> k = project<T>(a, b);
	return a - k;
}

template<std::floating_point T>
vec3<T> reflection(vec3<T>& v)
{
	vec2<T> n = v->normalized;
	
	return v - (2 * (dot<T>(v, n))) * n;
}

template<std::floating_point T>
vec3<T> lerp(vec3<T>& a, vec3<T>& b, const T t)
{
    return a + t*(b-a);
}

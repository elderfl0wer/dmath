#include <cmath>
#include <concepts>

#include "../../include/dmath/dmath.hpp"

template<std::floating_point T>
T dot(vec3<T>* a, vec3<T>* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

template<std::floating_point T>
vec3<T> cross(vec3<T>* a, vec3<T>* b)
{
	return { a->y * b->z - b->y * a->z, b->x * a->z - a->x * b->z, a->x * b->y - b->x * a->y };
}

template<std::floating_point T>
vec3<T> rotate(const vec3<T>& other, const double angle) {
	vec3<T> k = other.normalized();

	vec3<T> v{ x, y };

	vec3<T> k1 = v * cos(angle);
	vec3<T> k2 = cross<T>(k, v) * sin(angle);
	vec3<T> k3 = (dot<T>(k, v) * (1 - cos(angle))) * k;

	return k1 + k2 + k3;
}
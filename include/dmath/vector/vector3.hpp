#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <concepts>
#include <vector>

template<std::floating_point T>
class vec3 {
public:
	T x, y, z;

	T length() {
		return std::sqrt(x * x + y * y + z * z);
	}
	vec3<T> normalized() {
		const double factor{ 1 / std::sqrt(x * x + y * y + z * z) };
		return { x * factor, y * factor, z*factor};
	}
	void scale(const T factor) {
		x *= factor;
		y *= factor;
		z *= factor;
		return;
	}
	std::vector<T> dircos() {
		return { x / length(), y / length(), z / length()};
	}
	void rotate(const vec3<T>& other, const double angle) {
		vec3<T> k = other.normalized();

		vec3<T> v{ x, y, z };

		vec3<T> k1 = v * cos(angle);
		vec3<T> k2 = cross<T>(k, v) * sin(angle);
		vec3<T> k3 = (dot<T>(k, v) * (1 - cos(angle))) * k;
	
		x = k1->x + k2 > x + k3->x;
		y = k1->y + k2 > y + k3->y;
		z = k1->z + k2 > z + k3->z;
	}

	vec3<T> operator+ (const vec3<T>& other) {
		return { x + other.x, y + other.y, z + other.z };
	}
	vec3<T> operator- (const vec3<T>& other) {
		return { x - other.x, y - other.y, z - other.z };
	}
	vec3<T> operator* (const T factor) {
		return { x * factor, y * factor, z * factor };
	}
};

template<std::floating_point T>
T dot(vec3<T>* a, vec3<T>* b);
template<std::floating_point T>
vec3<T> cross(vec3<T>* a, vec3<T>* b);
template<std::floating_point T>
vec3<T> rotate(const vec3<T>& other, const double angle);
template<std::floating_point T>
T distance(vec3<T>* a, vec3<T>* b);
template<std::floating_point T>
T mutual_angle(vec3<T>* a, vec3<T>* b);
template<std::floating_point T>
vec3<T> project(vec3<T>* a, vec3<T>* b);
template<std::floating_point T>
vec3<T> orthogonal(vec3<T>* a, vec3<T>* b);
template<std::floating_point T>
vec3<T> reflection(vec3<T>* v);

#endif // !VECTOR3_HPP

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
	vec2<T> normalized() {
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

	vec2<T> operator+ (const vec2& other) {
		return { x + other.x, y + other.y, z + other.z };
	}
	vec2<T> operator- (const vec2& other) {
		return { x - other.x, y - other.y, z - other.z };
	}
	vec2<T> operator* (const T factor) {
		return { x * factor, y * factor, z * factor };
	}
};

#endif // !VECTOR3_HPP

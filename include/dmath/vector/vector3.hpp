#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <array>
#include <cmath>
#include <concepts>

namespace dmath {

template<std::floating_point T>
class vec3 {
public:
	T x, y, z;

	constexpr T length() const {
		return std::sqrt(x * x + y * y + z * z);
	}
	constexpr vec3<T> normalized() const {
		const double factor{ 1 / std::sqrt(x * x + y * y + z * z) };
		return { x * factor, y * factor, z*factor};
	}
	constexpr void scale(const T factor) {
		x *= factor;
		y *= factor;
		z *= factor;
		return;
	}
	constexpr std::array<T, 3> dircos() const {
		return { x / length(), y / length(), z / length()};
	}
	constexpr void rotate(const vec3<T>& other, const T angle) {
		vec3<T> k = other.normalized();

		vec3<T> v{ x, y, z };

		vec3<T> k1 = v * cos(angle);
		vec3<T> k2 = cross(k, v) * sin(angle);
		vec3<T> k3 = (dot(k, v) * (1 - cos(angle))) * k;
        
        vec3<T> result = k1 + k2 + k3;
		x = result.x;
		y = result.y;
		z = result.z;
	}

    static constexpr vec3<T> unit_x() {
        return {1.0f, 0, 0};
    }
    static constexpr vec3<T> unit_y() {
        return {0, 1.0f, 0};
    }
    static constexpr vec3<T> unit_z() {
        return {0, 0, 1.0f};
    }


	constexpr vec3<T> operator+(const vec3<T>& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}
	constexpr vec3<T> operator-(const vec3<T>& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}
	constexpr vec3<T> operator*(const T factor) const {
		return { x * factor, y * factor, z * factor };
	}
	constexpr bool operator==(const vec3& other) const {
		if ((*this).x == other.x && (*this).y == other.y && (*this).z == other.z) {
			return true;
		}
		return false;
	}
	constexpr auto operator<=>(const vec3& other) const = default;
};

using namespace dmath;

template<std::floating_point T>
T dot(vec3<T>& a, vec3<T>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<std::floating_point T>
vec3<T> cross(vec3<T>& a, vec3<T>& b)
{
	return { a.y * b.z - b.y * a.z, b.x * a.z - a.x * b.z, a.x * b.y - b.x * a.y };
}

template<std::floating_point T>
vec3<T> rotate(vec3<T>& v, const vec3<T>& other, const T angle) 
{
	vec3<T> k = other.normalized();

	vec3<T> k1 = v * cos(angle);
	vec3<T> k2 = cross(k, v) * sin(angle);
	vec3<T> k3 = (dot(k, v) * (1 - cos(angle))) * k;

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
    return std::acos(dot<T>(a, b) / (a.length()*b.length()));
}

template<std::floating_point T>
vec3<T> project(vec3<T>& a, vec3<T>& b)
{
    T k = dot<T>(a, b) / std::pow(b.length(), 2);

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
	vec3<T> n = v.normalized();
	
	return v - (2 * (dot<T>(v, n))) * n;
}

template<std::floating_point T>
vec3<T> lerp(vec3<T>& a, vec3<T>& b, const T t)
{
    return a + t*(b-a);
}


} /* namespace dmath */
#endif // !VECTOR3_HPP

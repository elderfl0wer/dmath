#include <iostream>
#include <cassert>
#include <cmath>

#include "../../include/dmath/vector/vector2.hpp"
#include "../../include/dmath/vector/vector3.hpp"

using namespace dmath;

int main() {
    vec2<float> v1{1.0f, 2.0f};
    using vec2f = vec2<float>;

    std::cout << "Starting dmath::vec2 tests..." << std::endl;

    // 1. Test construction and static factory methods
    v1 = {3.0f, 4.0f};
    assert(v1.x == 3.0f);
    assert(v1.y == 4.0f);

    auto ux = dmath::vec2f::unit_x();
    assert(ux.x == 1.0f && ux.y == 0.0f);

    auto uy = dmath::vec2f::unit_y();
    assert(uy.x == 0.0f && uy.y == 1.0f);

    // 2. Test member functions: length, normalized, scale, dircos, rotate
    assert(v1.length() == 5.0f);

    auto v1_norm = v1.normalized();
    assert(std::abs(v1_norm.length() - 1.0f) < 1e-5f);

    dmath::vec2f v2{1.0f, 2.0f};
    v2.scale(2.0f);
    assert(v2.x == 2.0f && v2.y == 4.0f);

    auto dc = v1.dircos();
    assert(dc[0] == 3.0f / 5.0f);
    assert(dc[1] == 4.0f / 5.0f);

    dmath::vec2f v_rot{1.0f, 0.0f};
    v_rot.rotate(3.1415926535f / 2.0f);
    assert(std::abs(v_rot.x) < 1e-5f);
    assert(std::abs(v_rot.y - 1.0f) < 1e-5f);

    // 3. Test operators (+, -, *, ==, <=>)
    dmath::vec2f a{1.0f, 2.0f};
    dmath::vec2f b{3.0f, 4.0f};

    auto sum = a + b;
    assert(sum.x == 4.0f && sum.y == 6.0f);

    auto diff = b - a;
    assert(diff.x == 2.0f && diff.y == 2.0f);

    auto scaled = a * 2.0f;
    assert(scaled.x == 2.0f && scaled.y == 4.0f);

    assert(a == (dmath::vec2f{1.0f, 2.0f}));
    assert((a <=> b) != std::strong_ordering::equal);

    // 4. Test non-member utility functions
    dmath::vec2f vec_a{1.0f, 0.0f};
    dmath::vec2f vec_b{0.0f, 1.0f};

    assert(dmath::dot(vec_a, vec_b) == 0.0f);
    assert(std::abs(dmath::distance(vec_a, vec_b) - std::sqrt(2.0f)) < 1e-5f);
    assert(std::abs(dmath::mutual_angle(vec_a, vec_b) - 3.1415926535f / 2.0f) < 1e-5f);

    auto crs = dmath::cross(vec_a, vec_b);
    assert(crs.z == 1.0f);

    dmath::vec2f proj_target{3.0f, 4.0f};
    dmath::vec2f proj_base{1.0f, 0.0f};

    auto proj = dmath::project(proj_target, proj_base);
    assert(proj.x == 3.0f && proj.y == 0.0f);

    auto ortho = dmath::orthogonal(proj_target, proj_base);
    assert(ortho.x == 0.0f && ortho.y == 4.0f);

    auto refl = dmath::reflection(vec_a);
    auto rot_fn = dmath::rotate(vec_a, 0.0f);

    auto lerp_fn = dmath::lerp(vec_a, vec_b, 0.5f);
    assert(lerp_fn.x == 0.5f && lerp_fn.y == 0.5f);

    std::cout << "All vector2 tests passed successfully!" << std::endl;
    return 0;
}
// ============================================================================
// vec2_test.cpp  --  brutal, assert-based test suite for vector2.hpp ONLY
//
// build : g++ -std=c++20 -g -Wall -Wextra vec2_test.cpp -o vec2_test
// run   : ./vec2_test              run everything
//         ./vec2_test rotate       run only tests whose name contains "rotate"
// extra : -DVEC2_TEST_PEDANTIC     also build robustness tests that go beyond
//         "the maths is right": const-correctness, overflow/underflow safe
//         length(), zero-vector handling.
//
// Every check is a real assert() (NDEBUG is force-undefined), so the first
// failure aborts and prints the file, line and the failing expression.
// Everything runs for float AND double. Deterministic fuzzing (fixed seed).
// ============================================================================
#undef NDEBUG
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <compare>
#include <concepts>
#include <cstdio>
#include <cstring>
#include <limits>
#include <random>
#include <type_traits>

#if __has_include("vector/vector2.hpp")
#  include "vector/vector2.hpp"
#else
#  include "../../include/dmath/vector/vector2.hpp"
#endif

using dmath::vec2;
using dmath::vec3;

// ----------------------------------------------------------------------------
// tiny harness
// ----------------------------------------------------------------------------
#define CHECK(cond)                                                            \
    do {                                                                       \
        if (!(cond))                                                           \
            std::fprintf(stderr, "\n  CHECK FAILED  %s:%d\n    %s\n",           \
                         __FILE__, __LINE__, #cond);                           \
        assert(cond);                                                          \
    } while (0)

static const char* g_filter = nullptr;

template<class T> const char* tname() {
    return std::is_same_v<T, float> ? "float" : "double";
}

#define RUN(name)                                                              \
    do {                                                                       \
        if (!g_filter || std::strstr(#name, g_filter)) {                       \
            std::printf("[ RUN  ] %-28s <%s>\n", #name, tname<T>());           \
            name<T>();                                                         \
            std::printf("[  OK  ] %s\n", #name);                               \
        }                                                                      \
    } while (0)

template<class T> inline constexpr T EPS = std::numeric_limits<T>::epsilon();
template<class T> inline constexpr T PI  = T(3.14159265358979323846264338327950288L);
inline constexpr double PI_D = 3.14159265358979323846264338327950288;

// |a-b| <= k * eps * mag   (mag = the magnitude the rounding error scales with)
template<std::floating_point T>
bool within(T a, std::type_identity_t<T> b, std::type_identity_t<T> mag, double k = 32) {
    if (std::isnan(a) || std::isnan(b)) return false;
    if (a == b) return true;
    return std::fabs(a - b) <= T(k) * EPS<T> * std::fmax(mag, std::numeric_limits<T>::min());
}
template<std::floating_point T>
bool within(const vec2<T>& a, const vec2<T>& b, std::type_identity_t<T> mag, double k = 32) {
    return within(a.x, b.x, mag, k) && within(a.y, b.y, mag, k);
}
// same, with mag = max(1, |a|, |b|)
template<std::floating_point T>
bool approx(T a, std::type_identity_t<T> b, double k = 64) {
    return within(a, b, std::fmax(T(1), std::fmax(std::fabs(a), std::fabs(b))), k);
}
template<std::floating_point T>
bool approx(const vec2<T>& a, const vec2<T>& b, double k = 64) {
    const T mag = std::fmax(T(1), std::fmax(a.length(), b.length()));
    return within(a.x, b.x, mag, k) && within(a.y, b.y, mag, k);
}

// deterministic random source
template<std::floating_point T>
struct Rng {
    std::mt19937_64 gen{0xD15EA5Eull};
    std::uniform_real_distribution<double> dist{-100.0, 100.0};
    T scalar() { return static_cast<T>(dist(gen)); }                               // [-100,100]
    T unit()   { return static_cast<T>((dist(gen) + 100.0) / 200.0); }             // [0,1]
    T angle()  { return static_cast<T>(dist(gen) / 100.0 * 3.141592653589793); }   // [-pi,pi]
    vec2<T> vec() { const T a = scalar(); const T b = scalar(); return vec2<T>{a, b}; }
    vec2<T> nonzero() {
        for (;;) { vec2<T> v = vec(); if (v.length() > T(1)) return v; }
    }
};
constexpr int FUZZ_N = 5000;

// ----------------------------------------------------------------------------
// compile-time contract
// ----------------------------------------------------------------------------
static_assert(std::is_aggregate_v<vec2<float>>);
static_assert(std::is_aggregate_v<vec2<double>>);
static_assert(std::is_trivially_copyable_v<vec2<double>>);
static_assert(std::is_trivially_default_constructible_v<vec2<double>>);
static_assert(std::is_standard_layout_v<vec2<double>>);
static_assert(sizeof(vec2<float>)  == 2 * sizeof(float));
static_assert(sizeof(vec2<double>) == 2 * sizeof(double));

template<class U> concept can_vec2 = requires { typename dmath::vec2<U>; };
static_assert(can_vec2<float> && can_vec2<double> && can_vec2<long double>);
static_assert(!can_vec2<int>);
static_assert(!can_vec2<unsigned>);
static_assert(!can_vec2<char>);
static_assert(!can_vec2<bool>);

template<class V> concept const_usable = requires(const V& c) {
    c.length();
    c.dircos();
    c + c;
    c - c;
    c * 2.0;
    { c == c } -> std::convertible_to<bool>;
    c <=> c;
};
static_assert(const_usable<vec2<float>> && const_usable<vec2<double>>);

constexpr vec2<double> ca{1, 2}, cb{3, 4};
static_assert(ca + cb == vec2<double>{4, 6});
static_assert(cb - ca == vec2<double>{2, 2});
static_assert(ca * 3.0 == vec2<double>{3, 6});
static_assert(ca != cb);
static_assert(ca < cb);
static_assert(vec2<double>::unit_x() == vec2<double>{1, 0});
static_assert(vec2<double>::unit_y() == vec2<double>{0, 1});
constexpr auto cscaled = [] { vec2<double> v{1, 2}; v.scale(3); return v; }();
static_assert(cscaled == vec2<double>{3, 6});

#ifdef VEC2_TEST_PEDANTIC
template<class V> concept const_normalizable = requires(const V& c) { c.normalized(); };
template<class V> concept free_fns_take_const = requires(const V& c) {
    dmath::dot(c, c); dmath::distance(c, c); dmath::mutual_angle(c, c);
    dmath::cross(c, c); dmath::project(c, c); dmath::orthogonal(c, c);
    dmath::reflection(c); dmath::lerp(c, c, 0.5);
};
template<class V> concept free_fns_take_temporaries = requires {
    dmath::dot(V{}, V{}); dmath::distance(V{}, V{}); dmath::project(V{}, V{});
};
static_assert(const_normalizable<vec2<double>>,        "normalized() should be a const member");
static_assert(free_fns_take_const<vec2<double>>,       "free functions should take const refs");
static_assert(free_fns_take_temporaries<vec2<double>>, "free functions should accept temporaries");
#endif

// ----------------------------------------------------------------------------
// construction / layout / unit vectors
// ----------------------------------------------------------------------------
template<class T> void test_construction() {
    vec2<T> z{};
    CHECK(z.x == 0 && z.y == 0);

    vec2<T> v{T(1.5), T(-2.5)};
    CHECK(v.x == T(1.5) && v.y == T(-2.5));

    vec2<T> c = v;
    CHECK(c == v);
    c.x = 9;
    CHECK(c != v);
    CHECK(v.x == T(1.5));          // copies are independent
    c = v;
    CHECK(c == v);

    const vec2<T> arr[3] = {{1, 2}, {3, 4}, {5, 6}};
    CHECK(arr[0].x == 1 && arr[1].y == 4 && arr[2].x == 5 && arr[2].y == 6);

    const vec2<T> negz{T(-0.0), T(0)};
    const vec2<T> posz{T(0), T(-0.0)};
    CHECK(negz == posz);           // -0 == +0
}

template<class T> void test_unit_vectors() {
    vec2<T> x = vec2<T>::unit_x();
    vec2<T> y = vec2<T>::unit_y();
    CHECK(x.x == 1 && x.y == 0);
    CHECK(y.x == 0 && y.y == 1);
    CHECK(x.length() == 1 && y.length() == 1);
    CHECK(dmath::dot(x, y) == 0);
    CHECK(dmath::dot(x, x) == 1);
    CHECK(dmath::dot(y, y) == 1);
    CHECK(dmath::cross(x, y).z == 1);
    CHECK(dmath::cross(y, x).z == -1);
    const vec2<T> xy{1, 1};
    CHECK(x + y == xy);
    CHECK(x != y);
    CHECK(approx(dmath::mutual_angle(x, y), PI<T> / 2));
    const auto dx = x.dircos();
    CHECK(dx[0] == 1 && dx[1] == 0);
    const auto dy = y.dircos();
    CHECK(dy[0] == 0 && dy[1] == 1);
    CHECK(x.normalized() == x);
    CHECK(y.normalized() == y);
}

// ----------------------------------------------------------------------------
// length
// ----------------------------------------------------------------------------
template<class T> void test_length() {
    struct Case { double x, y, len; };
    const Case cases[] = {
        {3, 4, 5},   {-3, 4, 5},  {3, -4, 5},  {-3, -4, 5}, {0, 0, 0},
        {1, 0, 1},   {0, 1, 1},   {0, -7, 7},  {-9, 0, 9},
        {5, 12, 13}, {8, 15, 17}, {7, 24, 25}, {20, 21, 29},
        {1, 1, 1.4142135623730951}, {0.6, 0.8, 1},
        {1e-3, 0, 1e-3}, {1e3, 0, 1e3}, {1e6, 1e6, 1.4142135623730951e6},
    };
    for (const auto& c : cases) {
        vec2<T> v{T(c.x), T(c.y)};
        CHECK(approx(v.length(), T(c.len)));
        CHECK(v.length() >= 0);
        vec2<T> neg{T(-c.x), T(-c.y)};
        vec2<T> swp{T(c.y), T(c.x)};
        CHECK(neg.length() == v.length());     // sign-blind
        CHECK(swp.length() == v.length());     // axis-blind
    }
    // exact pythagorean triples: sqrt is correctly rounded so these are exact
    CHECK((vec2<T>{3, 4}.length() == 5));
    CHECK((vec2<T>{5, 12}.length() == 13));
    CHECK((vec2<T>{8, 15}.length() == 17));

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> v = r.vec();
        const T l = v.length();
        CHECK(l >= std::fmax(std::fabs(v.x), std::fabs(v.y)) * (1 - 4 * EPS<T>));
        CHECK(l <= (std::fabs(v.x) + std::fabs(v.y)) * (1 + 4 * EPS<T>));
        const T k = r.scalar() / 10;
        CHECK(within((v * k).length(), std::fabs(k) * l, std::fabs(k) * l, 8));
    }

    if constexpr (std::is_same_v<T, double>) {
        vec2<T> big{1e150, 1e150};
        CHECK(within(big.length(), 1.4142135623730951e150, 1.4142135623730951e150, 8));
        vec2<T> small{1e-150, 1e-150};
        CHECK(within(small.length(), 1.4142135623730951e-150, 1.4142135623730951e-150, 8));
    }
}

// ----------------------------------------------------------------------------
// normalized
// ----------------------------------------------------------------------------
template<class T> void test_normalized() {
    {
        vec2<T> v{3, 4};
        vec2<T> n = v.normalized();
        const vec2<T> want{T(0.6), T(0.8)};
        CHECK(approx(n, want));
        const vec2<T> orig{3, 4};
        CHECK(v == orig);                       // returns a copy, doesn't mutate
    }
    {
        vec2<T> v{-5, 0};
        const vec2<T> want{-1, 0};
        CHECK(approx(v.normalized(), want));
    }
    {
        vec2<T> v{0, T(1e-3)};
        const vec2<T> want{0, 1};
        CHECK(approx(v.normalized(), want));
    }
    // full circle sweep, 1 degree steps
    for (int i = 0; i < 360; i++) {
        const T th = PI<T> * T(i) / 180;
        vec2<T> v{T(7.5) * std::cos(th), T(7.5) * std::sin(th)};
        vec2<T> n = v.normalized();
        const vec2<T> want{std::cos(th), std::sin(th)};
        CHECK(within(n.length(), 1, 1, 8));
        CHECK(approx(n, want));
    }
    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> v = r.nonzero();
        vec2<T> n = v.normalized();
        CHECK(within(n.length(), 1, 1, 8));
        CHECK(dmath::dot(n, v) > 0);                                   // same direction
        CHECK(within(dmath::cross(n, v).z, 0, v.length(), 16));       // parallel
        vec2<T> nn = n.normalized();
        CHECK(approx(nn, n, 8));                                       // idempotent
        vec2<T> pos = (v * T(3.5)).normalized();
        CHECK(approx(pos, n, 16));                                     // scale invariant
        vec2<T> neg = (v * T(-3.5)).normalized();
        const vec2<T> nflip{-n.x, -n.y};
        CHECK(approx(neg, nflip, 16));                                 // sign flips direction
    }
}

// ----------------------------------------------------------------------------
// scale
// ----------------------------------------------------------------------------
template<class T> void test_scale() {
    vec2<T> v{1, -2};
    v.scale(2);          CHECK((v.x == 2 && v.y == -4));
    v.scale(T(0.5));     CHECK((v.x == 1 && v.y == -2));
    v.scale(-1);         CHECK((v.x == -1 && v.y == 2));
    v.scale(1);          CHECK((v.x == -1 && v.y == 2));
    v.scale(0);          CHECK((v.x == 0 && v.y == 0));
    static_assert(std::is_void_v<decltype(v.scale(T(1)))>);

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> orig = r.vec();
        const T k = r.scalar() / 10;
        vec2<T> a = orig;
        a.scale(k);
        CHECK(a == orig * k);                      // scale() and operator* agree exactly
        CHECK(within(a.length(), std::fabs(k) * orig.length(), std::fabs(k) * orig.length(), 8));
        if (std::fabs(k) > T(1e-3)) {
            a.scale(T(1) / k);
            CHECK(within(a, orig, orig.length(), 8));
        }
    }
}

// ----------------------------------------------------------------------------
// dircos
// ----------------------------------------------------------------------------
template<class T> void test_dircos() {
    struct Case { double x, y, cx, cy; };
    const double h = 0.70710678118654752440;
    const Case cases[] = {
        {3, 4, 0.6, 0.8}, {-3, -4, -0.6, -0.8}, {3, -4, 0.6, -0.8},
        {0, 5, 0, 1}, {-2, 0, -1, 0}, {1, 1, h, h}, {-1, 1, -h, h}, {100, 0, 1, 0},
    };
    for (const auto& c : cases) {
        vec2<T> v{T(c.x), T(c.y)};
        auto d = v.dircos();
        static_assert(std::is_same_v<decltype(d), std::array<T, 2>>);
        CHECK(approx(d[0], T(c.cx)));
        CHECK(approx(d[1], T(c.cy)));
    }
    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> v = r.nonzero();
        auto d = v.dircos();
        CHECK(within(d[0] * d[0] + d[1] * d[1], 1, 1, 16));
        vec2<T> n = v.normalized();
        CHECK(approx(vec2<T>{d[0], d[1]}, n, 16));
        if (std::fabs(d[0]) < T(0.99)) {
            CHECK(within(std::acos(d[0]), std::fabs(std::atan2(v.y, v.x)), 4, 2048));
        }
    }
}

// ----------------------------------------------------------------------------
// member rotate()  (in-place)
// ----------------------------------------------------------------------------
template<class T> void test_rotate_member() {
    struct Case { double vx, vy, th, ex, ey; };
    const double h = 0.70710678118654752440, s2 = 1.4142135623730951, s3 = 1.7320508075688772;
    const Case cases[] = {
        {1, 0,  PI_D / 2,  0,  1},
        {0, 1,  PI_D / 2, -1,  0},
        {-1, 0, PI_D / 2,  0, -1},
        {1, 2,  PI_D / 2, -2,  1},
        {1, 2,  PI_D,     -1, -2},
        {1, 0, -PI_D / 2,  0, -1},
        {1, 0,  PI_D / 4,  h,  h},
        {1, 1,  PI_D / 4,  0,  s2},
        {2, 0,  PI_D / 6,  s3, 1},
        {3, 4,  0,         3,  4},
        {1, 0,  2 * PI_D,  1,  0},
        {5, -7, -PI_D,    -5,  7},
    };
    for (const auto& c : cases) {
        vec2<T> v{T(c.vx), T(c.vy)};
        v.rotate(T(c.th));
        const vec2<T> want{T(c.ex), T(c.ey)};
        CHECK(approx(v, want));
    }
    // unit_x swept around the circle must land on (cos, sin)
    for (int i = 0; i < 360; i++) {
        const T th = PI<T> * T(i) / 180;
        vec2<T> v = vec2<T>::unit_x();
        v.rotate(th);
        const vec2<T> want{std::cos(th), std::sin(th)};
        CHECK(approx(v, want));
    }
    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> v = r.vec();
        const T a = r.angle(), b = r.angle();

        vec2<T> w = v;
        w.rotate(a);
        CHECK(within(w.length(), v.length(), v.length(), 16));        // isometry
        CHECK(approx(w, dmath::rotate(v, a), 8));                     // agrees with free rotate

        w.rotate(-a);
        CHECK(within(w, v, v.length(), 32));                          // inverse restores

        vec2<T> p = v, q = v;
        p.rotate(a); p.rotate(b);
        q.rotate(a + b);
        CHECK(within(p, q, v.length(), 64));                          // composition
    }
}

// ----------------------------------------------------------------------------
// operators + - * == !=
// ----------------------------------------------------------------------------
template<class T> void test_operators() {
    vec2<T> a{1, 2}, b{3, -4};
    vec2<T> s = a + b;   CHECK((s.x == 4 && s.y == -2));
    vec2<T> d = a - b;   CHECK((d.x == -2 && d.y == 6));
    vec2<T> m = a * 3;   CHECK((m.x == 3 && m.y == 6));
    vec2<T> m2 = a * T(0.5); CHECK((m2.x == T(0.5) && m2.y == 1));

    vec2<T> chain = (a + b) - a;
    CHECK(chain == b);
    chain = a * 2 + b * 3 - a;
    const vec2<T> want{10, -10};
    CHECK(chain == want);

    const vec2<T> zero{0, 0};
    CHECK(a - a == zero);
    CHECK(a + zero == a);
    CHECK(a - zero == a);
    CHECK(a * 0 == zero);
    CHECK(a * 1 == a);
    CHECK(zero * 123 == zero);
    CHECK(zero - a == a * -1);
    CHECK(a + b == b + a);
    CHECK((a * T(2)) * T(0.5) == a);
    CHECK(a * 2 == a * T(2));                // int -> T conversion of the scalar
    a = a + a;
    const vec2<T> twice{2, 4};
    CHECK(a == twice);

    // equality semantics
    vec2<T> e{1, 2};
    CHECK(e == e);
    CHECK(!(e != e));
    vec2<T> ey = e; ey.y += 1;
    vec2<T> ex = e; ex.x += 1;
    CHECK(e != ey && !(e == ey));
    CHECK(e != ex && !(e == ex));
    CHECK(ey != ex);

    const T nan = std::numeric_limits<T>::quiet_NaN();
    const T inf = std::numeric_limits<T>::infinity();
    vec2<T> nx{nan, 0}, ny{0, nan};
    CHECK(nx != nx && !(nx == nx));          // NaN poisons equality, even with itself
    CHECK(ny != ny && !(ny == ny));
    CHECK(std::isnan((nx + e).x) && std::isnan((e - ny).y) && std::isnan((nx * 2).x));
    vec2<T> ix{inf, 0};
    CHECK((ix + ix).x == inf);
    CHECK(std::isnan((ix - ix).x));
    CHECK((ix * -1).x == -inf);
    CHECK(std::isnan((ix * 0).x));
    CHECK(std::isinf(ix.length()));
    CHECK(std::isnan(nx.length()));
    vec2<T> iy{-inf, inf};
    CHECK(iy.length() == inf);

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> p = r.vec(), q = r.vec(), t = r.vec();
        CHECK(p + q == q + p);
        CHECK(p - q == p + q * T(-1));                                     // exact in IEEE
        CHECK(p - p == zero);
        const T mpq  = p.length() + q.length();
        const T mpqt = mpq + t.length();
        CHECK(within((p + q) + t, p + (q + t), mpqt));                     // associativity (approx)
        CHECK(within((p + q) - q, p, mpq));
        const T k = r.scalar() / 10;
        CHECK(within((p + q) * k, p * k + q * k, mpq * std::fabs(k)));     // distributivity
        CHECK(within((p - q) * k, p * k - q * k, mpq * std::fabs(k)));
    }
}

// ----------------------------------------------------------------------------
// <=>  /  < <= > >=
// ----------------------------------------------------------------------------
template<class T> void test_ordering() {
    vec2<T> a{1, 2}, b{1, 3}, c{2, 0};
    CHECK(a < b);  CHECK(b < c);  CHECK(a < c);        // lexicographic: x first, then y
    CHECK(c > a);  CHECK(c > b);  CHECK(b > a);
    CHECK(a <= a); CHECK(a >= a); CHECK(a <= b); CHECK(c >= b);
    CHECK(!(a < a)); CHECK(!(a > a)); CHECK(!(b < a)); CHECK(!(a > b));
    CHECK((a <=> b) == std::partial_ordering::less);
    CHECK((c <=> b) == std::partial_ordering::greater);
    CHECK((a <=> a) == std::partial_ordering::equivalent);

    const T nan = std::numeric_limits<T>::quiet_NaN();
    vec2<T> nx{nan, 0}, ny{1, nan};
    CHECK((nx <=> a) == std::partial_ordering::unordered);
    CHECK((ny <=> a) == std::partial_ordering::unordered);   // x ties, y is NaN
    CHECK(!(nx < a) && !(nx > a) && !(nx <= a) && !(nx >= a) && !(nx == a));
    CHECK(!(ny < a) && !(ny > a) && !(ny <= a) && !(ny >= a) && !(ny == a));
    CHECK(nx != a && ny != a);
    vec2<T> lo{0, nan}, hi{1, 2};
    CHECK((lo <=> hi) == std::partial_ordering::less);        // x decides, NaN in y never looked at
    CHECK(lo < hi);

    // exhaustive over a tiny lattice: trichotomy + transitivity + consistency
    std::array<vec2<T>, 9> g;
    int n = 0;
    for (int x = -1; x <= 1; x++)
        for (int y = -1; y <= 1; y++) g[n++] = vec2<T>{T(x), T(y)};
    for (const auto& p : g)
        for (const auto& q : g) {
            CHECK(int(p < q) + int(p > q) + int(p == q) == 1);
            CHECK((p <= q) == (p < q || p == q));
            CHECK((p >= q) == (p > q || p == q));
            CHECK((p < q) == (q > p));
            CHECK((p == q) == !(p != q));
            for (const auto& s : g)
                if (p < q && q < s) CHECK(p < s);
        }

    Rng<T> r;
    std::array<vec2<T>, 128> arr;
    for (auto& v : arr) v = r.vec();
    auto less = [](const vec2<T>& p, const vec2<T>& q) { return p < q; };
    std::sort(arr.begin(), arr.end(), less);
    CHECK(std::is_sorted(arr.begin(), arr.end(), less));
    for (std::size_t i = 1; i < arr.size(); i++) CHECK(!(arr[i] < arr[i - 1]));
}

// ----------------------------------------------------------------------------
// dot
// ----------------------------------------------------------------------------
template<class T> void test_dot() {
    vec2<T> a{1, 2}, b{3, 4}, c{-2, 1}, o{0, 0};
    CHECK(dmath::dot(a, b) == 11);
    CHECK(dmath::dot(a, c) == 0);                       // orthogonal
    CHECK(dmath::dot(a, o) == 0);
    CHECK(dmath::dot(o, o) == 0);
    CHECK(dmath::dot(a, a) == 5);
    vec2<T> n{-1, -2};
    CHECK(dmath::dot(a, n) == -5);

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> p = r.vec(), q = r.vec(), t = r.vec();
        const T lp = p.length(), lq = q.length(), lt = t.length();
        const T d = dmath::dot(p, q);
        CHECK(d == dmath::dot(q, p));                                       // exact commutativity
        CHECK(std::fabs(d) <= lp * lq * (1 + 8 * EPS<T>));                  // Cauchy-Schwarz
        CHECK(within(dmath::dot(p, p), lp * lp, lp * lp, 8));
        const T k = r.scalar() / 10;
        vec2<T> pk = p * k;
        CHECK(within(dmath::dot(pk, q), k * d, std::fabs(k) * lp * lq));    // homogeneity
        vec2<T> pt = p + t;
        CHECK(within(dmath::dot(pt, q), dmath::dot(p, q) + dmath::dot(t, q), (lp + lt) * lq));
        // polarisation: |p+q|^2 = |p|^2 + 2 p.q + |q|^2
        vec2<T> pq = p + q;
        CHECK(within(dmath::dot(pq, pq), lp * lp + 2 * d + lq * lq, (lp + lq) * (lp + lq), 64));
    }
}

// ----------------------------------------------------------------------------
// distance
// ----------------------------------------------------------------------------
template<class T> void test_distance() {
    vec2<T> o{0, 0}, p{3, 4}, q{-3, -4};
    CHECK(dmath::distance(o, p) == 5);
    CHECK(dmath::distance(p, o) == 5);
    CHECK(dmath::distance(p, q) == 10);
    CHECK(dmath::distance(p, p) == 0);
    CHECK(dmath::distance(o, o) == 0);
    vec2<T> u{1, 1}, w{4, 5};
    CHECK(dmath::distance(u, w) == 5);

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> a = r.vec(), b = r.vec(), c = r.vec(), t = r.vec();
        const T dab = dmath::distance(a, b), dbc = dmath::distance(b, c), dac = dmath::distance(a, c);
        CHECK(dab >= 0);
        CHECK(dab == dmath::distance(b, a));                          // symmetric (exact)
        CHECK(dab == (a - b).length());
        CHECK(dmath::distance(a, a) == 0);
        CHECK(dac <= (dab + dbc) * (1 + 16 * EPS<T>));                // triangle inequality
        vec2<T> at = a + t, bt = b + t;
        CHECK(within(dmath::distance(at, bt), dab, a.length() + b.length() + t.length(), 32));
        vec2<T> o{0, 0};
        CHECK(dmath::distance(a, o) == a.length());
    }
}

// ----------------------------------------------------------------------------
// mutual_angle
// ----------------------------------------------------------------------------
template<class T> void test_mutual_angle() {
    // acos is ill-conditioned near 0 and pi: error ~ sqrt(eps), not eps
    const T loose = 16 * std::sqrt(EPS<T>);

    struct Case { double ax, ay, bx, by, ang; bool edge; };
    const Case cases[] = {
        {1, 0,  0, 1,  PI_D / 2,     false},
        {0, 1,  1, 0,  PI_D / 2,     false},
        {1, 0,  1, 1,  PI_D / 4,     false},
        {1, 0, -1, 1,  3 * PI_D / 4, false},
        {2, 0,  0, 5,  PI_D / 2,     false},
        {1, 1, -1, 1,  PI_D / 2,     false},
        {1, 1, -1, -1, PI_D,         true },
        {1, 0, -1, 0,  PI_D,         true },
        {1, 0,  1, 0,  0,            true },
        {3, 4,  3, 4,  0,            true },
        {3, 4,  6, 8,  0,            true },
        {1, 0,  1.7320508075688772, 1, PI_D / 6, false},
        {1, 0,  1, 1.7320508075688772, PI_D / 3, false},
    };
    for (const auto& c : cases) {
        vec2<T> a{T(c.ax), T(c.ay)}, b{T(c.bx), T(c.by)};
        const T ang = dmath::mutual_angle(a, b);
        CHECK(!std::isnan(ang));
        if (c.edge) CHECK(std::fabs(ang - T(c.ang)) <= loose);
        else        CHECK(approx(ang, T(c.ang)));
        CHECK(dmath::mutual_angle(a, b) == dmath::mutual_angle(b, a));     // symmetric
    }

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> a = r.nonzero(), b = r.nonzero();
        const T ang = dmath::mutual_angle(a, b);
        CHECK(!std::isnan(ang));
        CHECK(ang >= 0 && ang <= PI<T> * (1 + 4 * EPS<T>));
        CHECK(ang == dmath::mutual_angle(b, a));

        // cos(angle) * |a||b| == a.b
        const T la = a.length(), lb = b.length();
        CHECK(within(std::cos(ang) * la * lb, dmath::dot(a, b), la * lb, 64));

        // scale invariance (only where acos is well-conditioned)
        if (ang > T(0.2) && ang < T(2.9)) {
            vec2<T> a2 = a * T(2.5), b2 = b * T(0.125);
            CHECK(within(dmath::mutual_angle(a2, b2), ang, 4, 2048));
        }
        // angle between v and rotate(v, theta) is |theta|
        const T th = r.angle();
        if (std::fabs(th) > T(0.2) && std::fabs(th) < T(2.9)) {
            vec2<T> rot = dmath::rotate(a, th);
            CHECK(within(dmath::mutual_angle(a, rot), std::fabs(th), 4, 2048));
        }
    }

    // parallel / anti-parallel vectors: rounding can push dot/(|a||b|) to 1+eps
    // or -1-eps, and acos(1+eps) is NaN. 20000 pairs to flush that out.
    for (int i = 0; i < 4 * FUZZ_N; i++) {
        vec2<T> a = r.nonzero();
        const T k = std::fabs(r.scalar()) / 10 + T(0.01);
        vec2<T> same = a * k, opp = a * -k;
        const T t1 = dmath::mutual_angle(a, same);
        const T t2 = dmath::mutual_angle(a, opp);
        CHECK(!std::isnan(t1));
        CHECK(!std::isnan(t2));
        CHECK(t1 >= 0 && t1 <= loose);
        CHECK(std::fabs(t2 - PI<T>) <= loose);
    }
}

// ----------------------------------------------------------------------------
// cross  (2D -> vec3 with only z populated)
// ----------------------------------------------------------------------------
template<class T> void test_cross() {
    vec2<T> a{2, 3}, b{4, 5};
    static_assert(std::is_same_v<decltype(dmath::cross(a, b)), vec3<T>>);
    auto c = dmath::cross(a, b);
    CHECK(c.x == 0 && c.y == 0);
    CHECK(c.z == -2);                            // 2*5 - 3*4
    CHECK(dmath::cross(b, a).z == 2);
    CHECK(dmath::cross(a, a).z == 0);
    vec2<T> a2{4, 6};
    CHECK(dmath::cross(a, a2).z == 0);           // parallel
    vec2<T> an{-2, -3};
    CHECK(dmath::cross(a, an).z == 0);           // anti-parallel
    vec2<T> o{0, 0};
    CHECK(dmath::cross(a, o).z == 0);
    CHECK(dmath::cross(o, a).z == 0);

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> p = r.vec(), q = r.vec();
        auto pq = dmath::cross(p, q);
        auto qp = dmath::cross(q, p);
        CHECK(pq.x == 0 && pq.y == 0);
        CHECK(pq.z == -qp.z);                                            // anti-commutative, exact
        CHECK(dmath::cross(p, p).z == 0);
        const T lp = p.length(), lq = q.length();
        const T d = dmath::dot(p, q);
        // Lagrange identity: (p x q)^2 + (p . q)^2 == |p|^2 |q|^2
        CHECK(within(pq.z * pq.z + d * d, lp * lp * lq * lq, lp * lp * lq * lq, 64));
        CHECK(std::fabs(pq.z) <= lp * lq * (1 + 8 * EPS<T>));
        // signed area under rotation: p x rot(p, th) = |p|^2 sin(th)
        const T th = r.angle();
        vec2<T> rot = dmath::rotate(p, th);
        CHECK(within(dmath::cross(p, rot).z, lp * lp * std::sin(th), lp * lp, 64));
    }
}

// ----------------------------------------------------------------------------
// project / orthogonal
// ----------------------------------------------------------------------------
template<class T> void test_project_orthogonal() {
    {
        vec2<T> a{3, 4}, bx{1, 0}, bx5{5, 0}, by{0, 2}, bn{-1, 0}, bd{1, 1};
        const vec2<T> px{3, 0}, py{0, 4};
        CHECK(approx(dmath::project(a, bx), px));
        CHECK(approx(dmath::project(a, bx5), px));      // |b| must not matter
        CHECK(approx(dmath::project(a, bn), px));       // sign of b must not matter
        CHECK(approx(dmath::project(a, by), py));
        vec2<T> a2{2, 0};
        const vec2<T> pd{1, 1};
        CHECK(approx(dmath::project(a2, bd), pd));
        CHECK(approx(dmath::project(a, a), a));         // project onto itself
        vec2<T> ortho{-4, 3};
        const vec2<T> zero{0, 0};
        CHECK(approx(dmath::project(ortho, a), zero));  // orthogonal -> 0
        vec2<T> z{0, 0};
        CHECK(approx(dmath::project(z, a), zero));

        const vec2<T> ox{0, 4};
        CHECK(approx(dmath::orthogonal(a, bx), ox));
        CHECK(approx(dmath::orthogonal(a, a), zero));
        CHECK(approx(dmath::orthogonal(ortho, a), ortho));
    }
    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> a = r.vec(), b = r.nonzero();
        const T la = a.length(), lb = b.length();
        vec2<T> p = dmath::project(a, b);
        vec2<T> o = dmath::orthogonal(a, b);

        CHECK(within(p + o, a, la, 32));                                   // decomposition
        CHECK(within(dmath::cross(p, b).z, 0, la * lb, 32));              // p is parallel to b
        CHECK(within(dmath::dot(o, b), 0, la * lb, 64));                  // o is perpendicular to b
        CHECK(within(la * la, p.length() * p.length() + o.length() * o.length(), la * la, 64));
        CHECK(p.length() <= la * (1 + 8 * EPS<T>));                        // projection never grows
        vec2<T> pp = dmath::project(p, b);
        CHECK(within(pp, p, la, 64));                                      // idempotent
        vec2<T> b2 = b * T(3.25);
        CHECK(within(dmath::project(a, b2), p, la, 64));                   // scale of b irrelevant
    }
}

// ----------------------------------------------------------------------------
// reflection(v)   one-argument form: v - 2 (v.n) n with n = v/|v|  ==>  -v
// ----------------------------------------------------------------------------
template<class T> void test_reflection() {
    {
        vec2<T> v{3, 4};
        const vec2<T> want{-3, -4};
        CHECK(approx(dmath::reflection(v), want));
        vec2<T> x{1, 0};
        const vec2<T> wx{-1, 0};
        CHECK(approx(dmath::reflection(x), wx));
        vec2<T> y{0, -7};
        const vec2<T> wy{0, 7};
        CHECK(approx(dmath::reflection(y), wy));
    }
    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> v = r.nonzero();
        vec2<T> m = dmath::reflection(v);
        const vec2<T> neg{-v.x, -v.y};
        CHECK(within(m, neg, v.length(), 32));
        CHECK(within(m.length(), v.length(), v.length(), 16));             // isometry
        vec2<T> mm = dmath::reflection(m);
        CHECK(within(mm, v, v.length(), 64));                              // involution
    }
}

// ----------------------------------------------------------------------------
// free rotate(v, angle)
// ----------------------------------------------------------------------------
template<class T> void test_rotate_free() {
    struct Case { double vx, vy, th, ex, ey; };
    const double h = 0.70710678118654752440, s2 = 1.4142135623730951, s3 = 1.7320508075688772;
    const Case cases[] = {
        {1, 0,  PI_D / 2,  0,  1},
        {0, 1,  PI_D / 2, -1,  0},
        {-1, 0, PI_D / 2,  0, -1},
        {0, -1, PI_D / 2,  1,  0},
        {1, 2,  PI_D / 2, -2,  1},
        {1, 2,  PI_D,     -1, -2},
        {1, 0, -PI_D / 2,  0, -1},
        {1, 0,  PI_D / 4,  h,  h},
        {1, 1,  PI_D / 4,  0,  s2},
        {2, 0,  PI_D / 6,  s3, 1},
        {3, 4,  0,         3,  4},
        {1, 0,  2 * PI_D,  1,  0},
        {5, -7, -PI_D,    -5,  7},
        {0, 0,  1.234,     0,  0},
    };
    for (const auto& c : cases) {
        vec2<T> v{T(c.vx), T(c.vy)};
        const vec2<T> before = v;
        const T th = T(c.th);
        vec2<T> out = dmath::rotate(v, th);
        const vec2<T> want{T(c.ex), T(c.ey)};
        CHECK(approx(out, want));
        CHECK(v == before);                                    // input must not be modified
    }
    {
        vec2<T> v{3, 4};
        CHECK(dmath::rotate(v, T(0)) == v);                    // exact identity at 0
    }
    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> p = r.vec(), q = r.vec();
        const T a = r.angle(), b = r.angle();
        const T lp = p.length(), lq = q.length();
        vec2<T> rp = dmath::rotate(p, a);
        vec2<T> rq = dmath::rotate(q, a);

        CHECK(within(rp.length(), lp, lp, 16));                                  // isometry
        CHECK(within(dmath::dot(rp, rq), dmath::dot(p, q), lp * lq, 64));        // preserves dot
        CHECK(within(dmath::cross(rp, rq).z, dmath::cross(p, q).z, lp * lq, 64));// preserves orientation/area
        vec2<T> pq = p + q;
        CHECK(within(dmath::rotate(pq, a), rp + rq, lp + lq, 64));               // linear
        vec2<T> sp = p * T(3);
        CHECK(within(dmath::rotate(sp, a), rp * T(3), lp, 64));                  // commutes with scaling
        vec2<T> back = dmath::rotate(rp, -a);
        CHECK(within(back, p, lp, 64));                                          // inverse
        vec2<T> rab = dmath::rotate(rp, b);
        CHECK(within(rab, dmath::rotate(p, a + b), lp, 64));                     // composition
        CHECK(within(dmath::rotate(rp, PI<T> * 2 - a), p, lp, 64));              // full turn
    }
}

// ----------------------------------------------------------------------------
// lerp
// ----------------------------------------------------------------------------
template<class T> void test_lerp() {
    vec2<T> a{0, 0}, b{10, 20};
    const vec2<T> half{5, 10}, quarter{T(2.5), 5}, ext{20, 40}, neg{-10, -20};
    CHECK(dmath::lerp(a, b, T(0)) == a);
    CHECK(approx(dmath::lerp(a, b, T(1)), b));
    CHECK(approx(dmath::lerp(a, b, T(0.5)), half));
    CHECK(approx(dmath::lerp(a, b, T(0.25)), quarter));
    CHECK(approx(dmath::lerp(a, b, T(2)), ext));          // extrapolation
    CHECK(approx(dmath::lerp(a, b, T(-1)), neg));
    vec2<T> p{-3, 7};
    CHECK(dmath::lerp(p, p, T(0.37)) == p);               // degenerate segment

    Rng<T> r;
    for (int i = 0; i < FUZZ_N; i++) {
        vec2<T> p1 = r.vec(), p2 = r.vec();
        const T t = r.unit();
        const T mag = p1.length() + p2.length();
        vec2<T> m = dmath::lerp(p1, p2, t);

        CHECK(dmath::lerp(p1, p2, T(0)) == p1);                              // exact at t=0
        CHECK(within(dmath::lerp(p1, p2, T(1)), p2, mag, 8));
        vec2<T> rev = dmath::lerp(p2, p1, T(1) - t);
        CHECK(within(m, rev, mag, 16));                                      // symmetric
        CHECK(within(dmath::distance(p1, m), t * dmath::distance(p1, p2), mag, 16));
        CHECK(within(dmath::distance(m, p2), (T(1) - t) * dmath::distance(p1, p2), mag, 16));
        // lies on the segment
        const T seg = dmath::distance(p1, p2);
        vec2<T> d1 = m - p1, d2 = p2 - p1;
        CHECK(within(dmath::cross(d1, d2).z, 0, seg * seg + mag * mag, 32));
        // box containment
        const T slack = 8 * EPS<T> * mag;
        CHECK(m.x >= std::fmin(p1.x, p2.x) - slack && m.x <= std::fmax(p1.x, p2.x) + slack);
        CHECK(m.y >= std::fmin(p1.y, p2.y) - slack && m.y <= std::fmax(p1.y, p2.y) + slack);
    }
}

// ----------------------------------------------------------------------------
// optional robustness tests (-DVEC2_TEST_PEDANTIC)
// ----------------------------------------------------------------------------
#ifdef VEC2_TEST_PEDANTIC
template<std::floating_point T> bool all_finite(T v) { return std::isfinite(v); }
template<std::floating_point T> bool all_finite(const vec2<T>& v) {
    return std::isfinite(v.x) && std::isfinite(v.y);
}
template<std::floating_point T> bool all_finite(const std::array<T, 2>& v) {
    return std::isfinite(v[0]) && std::isfinite(v[1]);
}
template<class F> bool finite_or_throws(F&& f) {
    try { return all_finite(f()); } catch (...) { return true; }
}

template<class T> void test_pedantic_overflow_underflow() {
    const T big = std::sqrt(std::numeric_limits<T>::max()) * 2;      // big*big overflows, length fits
    vec2<T> v{big, big};
    CHECK(std::isfinite(v.length()));
    CHECK(within(v.length(), big * T(1.4142135623730951), big, 16));

    const T tiny = std::is_same_v<T, float> ? T(1e-30) : T(1e-170);  // tiny*tiny underflows to 0
    vec2<T> w{tiny, tiny};
    CHECK(w.length() > 0);
    CHECK(within(w.length(), tiny * T(1.4142135623730951), tiny, 16));
    CHECK(within(w.normalized().length(), 1, 1, 16));
}

template<class T> void test_pedantic_zero_vectors() {
    // a zero vector has no direction: acceptable outcomes are "throw" or "finite result"
    vec2<T> z{0, 0}, one{1, 0};
    CHECK(finite_or_throws([&] { return z.normalized(); }));
    CHECK(finite_or_throws([&] { return z.dircos(); }));
    CHECK(finite_or_throws([&] { return dmath::project(one, z); }));
    CHECK(finite_or_throws([&] { return dmath::orthogonal(one, z); }));
    CHECK(finite_or_throws([&] { return dmath::mutual_angle(one, z); }));
    CHECK(finite_or_throws([&] { return dmath::mutual_angle(z, z); }));
    CHECK(finite_or_throws([&] { return dmath::reflection(z); }));
}
#endif

// ----------------------------------------------------------------------------
template<class T> void run_all() {
    RUN(test_construction);
    RUN(test_unit_vectors);
    RUN(test_length);
    RUN(test_normalized);
    RUN(test_scale);
    RUN(test_dircos);
    RUN(test_rotate_member);
    RUN(test_operators);
    RUN(test_ordering);
    RUN(test_dot);
    RUN(test_distance);
    RUN(test_mutual_angle);
    RUN(test_cross);
    RUN(test_project_orthogonal);
    RUN(test_reflection);
    RUN(test_rotate_free);
    RUN(test_lerp);
#ifdef VEC2_TEST_PEDANTIC
    RUN(test_pedantic_overflow_underflow);
    RUN(test_pedantic_zero_vectors);
#endif
}

int main(int argc, char** argv) {
    if (argc > 1) g_filter = argv[1];
    run_all<float>();
    run_all<double>();
    std::puts("\nALL vec2 TESTS PASSED");
    return 0;
}
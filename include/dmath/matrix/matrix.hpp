#include <array>
#include <concepts>
#include <cstddef>

namespace dmath {

template<std::floating_point T, std::size_t Rows, std::size_t Cols>
class matrix {
public:
    std::array<std::array<T, Cols>, Rows> data{};

    T& operator()(std::size_t row, std::size_t col) {
        return data[row][col];
    }

    const T& operator()(std::size_t row, std::size_t col) const {
        return data[row][col];
    }
};
}

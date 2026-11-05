#ifndef QUANTITY_HPP
#define QUANTITY_HPP

namespace dmath {
template<typename T, class Unit>
class quantity
{
public:
    T data;
    using quantity_t = quantity<T, Unit>;

    constexpr quantity_t operator+(const quantity_t& other) const {
        return {.data = other.data+(*this).data};
    }
    constexpr quantity_t operator-(const quantity_t& other) const {
        return {.data = other.data-(*this).data};
    }
    constexpr bool operator==(const quantity_t& other) const {
        return (this->data == other.data) ? true : false;
    }
    constexpr auto operator<=>(const quantity_t& other) const = default;
};
} // !NAMESPACE DMATH

#endif // QUANTITY_HPP

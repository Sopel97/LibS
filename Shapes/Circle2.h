#pragma once

#include "Vec2.h"
#include "Box2.h"

#include <type_traits>

namespace ls
{
    template <typename T>
    struct Circle2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        Vec2<T> origin;
        T radius;

        constexpr Circle2() noexcept = default;

        constexpr Circle2(const Circle2<T>&) = default;
        constexpr Circle2(Circle2<T>&&) noexcept = default;

        Circle2<T>& operator =(Circle2<T> &&) noexcept = default;
        Circle2<T>& operator =(const Circle2<T>&) = default;

        constexpr Circle2(const Vec2<T>& origin, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value && std::is_nothrow_copy_constructible<T>::value) :
            origin(origin),
            radius(radius)
        {
        }

        constexpr Circle2<T> translated(const Vec2<T>& displacement) const
        {
            return Circle2<T>(origin + displacement, radius);
        }
        Circle2<T>& translate(const Vec2<T>& displacement)
        {
            origin += displacement;
            return *this;
        }

        constexpr Box2<T> aabb() const
        {
            const Vec2<T> halfDiagonal(radius, radius);
            return Box2<T>(origin - halfDiagonal, origin + halfDiagonal);
        }

        template <typename T2>
        constexpr explicit operator Circle2<T2>() const
        {
            return Circle2<T2>(static_cast<Vec2<T2>>(origin), static_cast<T2>(radius));
        }

        constexpr T area() const
        {
            return radius * radius * pi<T>;
        }
    };

    using Circle2D = Circle2<double>;
    using Circle2F = Circle2<float>;

    template <typename T>
    constexpr bool operator==(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        return std::tie(lhs.origin, lhs.radius) == std::tie(rhs.origin, rhs.radius);
    }
    template <typename T>
    constexpr bool operator!=(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

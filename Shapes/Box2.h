#pragma once

#include "Vec2.h"

#include <type_traits>

namespace ls
{
    template <typename T>
    struct Box2
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        Vec2<T> min, max;

        constexpr Box2() noexcept = default;

        constexpr Box2(const Box2<T>&) = default;
        constexpr Box2(Box2<T>&&) noexcept = default;

        Box2<T>& operator =(const Box2<T>&) = default;
        Box2<T>& operator =(Box2<T> &&) noexcept = default;

        constexpr Box2(const Vec2<T>& min, const Vec2<T>& max) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
            min(min),
            max(max)
        {
        }

        static constexpr Box2<T> withSize(const Vec2<T>& min, const Vec2<T>& size) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value)
        {
            return Box2<T>(min, min + size);
        }

        static constexpr const Box2<T>& unitRectangle() noexcept
        {
            static constexpr Box2<T> rect(Vec2<T>(T(0), T(0)), Vec2<T>(T(1), T(1)));
            return rect;
        }

        constexpr Box2<T> translated(const Vec2<T>& displacement) const
        {
            return Box2<T>(min + displacement, max + displacement);
        }

        Box2<T>& translate(const Vec2<T>& displacement)
        {
            min += displacement;
            max += displacement;
            return *this;
        }

        template <typename T2>
        constexpr explicit operator Box2<T2>() const
        {
            return Box2<T2>(static_cast<Vec2<T2>>(min), static_cast<Vec2<T2>>(max));
        }

        constexpr T width() const
        {
            return max.x - min.x;
        }

        constexpr T height() const
        {
            return max.y - min.y;
        }

        constexpr Vec2<T> centerOfMass() const
        {
            return (min + max) * T(0.5);
        }

        constexpr T area() const
        {
            return width()*height();
        }
    };

    using Box2D = Box2<double>;
    using Box2F = Box2<float>;
    using Box2I = Box2<int>;

    template <typename T>
    constexpr bool operator== (const Box2<T>& lhs, const Box2<T>& rhs)
    {
        return std::tie(lhs.min, lhs.max) == std::tie(rhs.min, rhs.max);
    }
    template <typename T>
    constexpr bool operator!= (const Box2<T>& lhs, const Box2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

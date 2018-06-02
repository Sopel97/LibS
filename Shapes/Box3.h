#pragma once

#include "Vec3.h"

#include <type_traits>

namespace ls
{
    template <typename T>
    struct Box3
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        Vec3<T> min, max;

        constexpr Box3() noexcept = default;

        constexpr Box3(const Box3<T>&) = default;
        constexpr Box3(Box3<T>&&) noexcept = default;

        Box3<T>& operator =(const Box3<T>&) = default;
        Box3<T>& operator =(Box3<T> &&) noexcept = default;

        constexpr Box3(const Vec3<T>& min, const Vec3<T>& max) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value) :
            min(min),
            max(max)
        {
        }

        static constexpr Box3<T> withSize(const Vec3<T>& min, const Vec3<T>& size) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value)
        {
            return Box3<T>(min, min + size);
        }
        static constexpr const Box3<T>& unitRectangle() noexcept
        {
            static constexpr Box3<T> rect(Vec3<T>(T(0), T(0), T(0)), Vec3<T>(T(1), T(1), T(1)));
            return rect;
        }

        constexpr Box3<T> translated(const Vec3<T>& displacement) const
        {
            return Box3<T>(min + displacement, max + displacement);
        }
        Box3<T>& translate(const Vec3<T>& displacement)
        {
            min += displacement;
            max += displacement;
            return *this;
        };

        template <typename T2>
        constexpr explicit operator Box3<T2>() const
        {
            return Box3<T2>(static_cast<Vec3<T2>>(min), static_cast<Vec3<T2>>(max));
        }

        constexpr T width() const
        {
            return max.x - min.x;
        }
        constexpr T height() const
        {
            return max.y - min.y;
        }
        constexpr T depth() const
        {
            return max.z - min.z;
        }

        constexpr Vec3<T> centerOfMass() const
        {
            return (min + max) * T(0.5);
        }
        constexpr T volume() const
        {
            return width()*height()*depth();
        }
    };

    using Box3D = Box3<double>;
    using Box3F = Box3<float>;
    using Box3I = Box3<int>;

    template <typename T>
    constexpr bool operator== (const Box3<T>& lhs, const Box3<T>& rhs)
    {
        return std::tie(lhs.min, lhs.max) == std::tie(rhs.min, rhs.max);
    }
    template <typename T>
    constexpr bool operator!= (const Box3<T>& lhs, const Box3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

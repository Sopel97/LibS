#pragma once

#include "Vec3.h"

#include <type_traits>
#include <tuple>

namespace ls
{
    // base is parallel to XZ axis
    template <typename T>
    struct Cylinder3
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        Vec3<T> baseOrigin;
        T height;
        T radius;

        constexpr Cylinder3() noexcept = default;
        
        constexpr Cylinder3(const Cylinder3<T>&) = default;
        constexpr Cylinder3(Cylinder3<T>&&) noexcept = default;

        Cylinder3<T>& operator =(Cylinder3<T> &&) noexcept = default;
        Cylinder3<T>& operator =(const Cylinder3<T>&) = default;

        constexpr Cylinder3(const Vec3<T>& baseOrigin, const T& height, const T& radius) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value && std::is_nothrow_copy_constructible<T>::value) :
            baseOrigin(baseOrigin),
            height(height),
            radius(radius)
        {
        }

        constexpr Cylinder3<T> translated(const Vec3<T>& displacement) const
        {
            return Cylinder3<T>(baseOrigin + displacement, height, radius);
        }

        Cylinder3<T>& translate(const Vec3<T>& displacement)
        {
            baseOrigin += displacement;
            return *this;
        }

        template <typename T2>
        constexpr explicit operator Cylinder3<T2>() const
        {
            return Cylinder3<T2>(static_cast<Vec3<T2>>(origin), static_cast<T2>(height), static_cast<T2>(radius));
        }
    };

    using Cylinder3D = Cylinder3<double>;
    using Cylinder3F = Cylinder3<float>;

    template <typename T>
    constexpr bool operator==(const Cylinder3<T>& lhs, const Cylinder3<T>& rhs)
    {
        return std::tie(lhs.origin, lhs.height, lhs.radius) == std::tie(rhs.origin, rhs.height, rhs.radius);
    }
    template <typename T>
    constexpr bool operator!=(const Cylinder3<T>& lhs, const Cylinder3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

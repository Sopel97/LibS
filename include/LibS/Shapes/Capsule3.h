#pragma once

#include "Vec3.h"
#include "Edge3.h"

#include <array>
#include <type_traits>
#include <tuple>

namespace ls
{
    template <typename T>
    struct Capsule3
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        Edge3<T> extent;
        T radius;

        constexpr Capsule3() noexcept = default;

        constexpr Capsule3(const Capsule3<T>&) = default;
        constexpr Capsule3(Capsule3<T>&&) noexcept = default;

        Capsule3<T>& operator=(const Capsule3<T>&) = default;
        Capsule3<T>& operator=(Capsule3<T> &&) noexcept = default;

        constexpr Capsule3(const Edge3<T>& extent, const T& radius) noexcept(std::is_nothrow_copy_constructible<Edge3<T>>::value) :
            extent(extent),
            radius(radius)
        {
        }

        constexpr Capsule3<T> translated(const Vec3<T>& displacement) const
        {
            return Capsule3<T>(extent.translated(displacement), radius);
        }
        constexpr Capsule3<T>& translate(const Vec3<T>& displacement)
        {
            extent.translate(displacement);

            return *this;
        }

        template <typename T2>
        explicit operator Capsule3<T2>() const
        {
            return Capsule3<T2>(static_cast<Vec3<T2>>(extent[0]), static_cast<Vec3<T2>>(extent[1]));
        }

        constexpr Vec3<T> centerOfMass() const
        {
            return extent.centerOfMass();
        }
    };

    using Capsule3D = Capsule3<double>;
    using Capsule3F = Capsule3<float>;

    template <typename T>
    constexpr bool operator== (const Capsule3<T>& lhs, const Capsule3<T>& rhs)
    {
        return std::tie(lhs.extent, lhs.radius) == std::tie(rhs.extent, rhs.radius);
    }
    template <typename T>
    constexpr bool operator!= (const Capsule3<T>& lhs, const Capsule3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

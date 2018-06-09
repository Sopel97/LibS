#pragma once

#include "Vec3.h"

#include <array>
#include <type_traits>
#include <tuple>

namespace ls
{
    template <typename T>
    struct Edge3
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        std::array<Vec3<T>, 2u> vertices;

        constexpr Edge3() noexcept = default;

        constexpr Edge3(const Edge3<T>&) = default;
        constexpr Edge3(Edge3<T>&&) noexcept = default;

        Edge3<T>& operator=(const Edge3<T>&) = default;
        Edge3<T>& operator=(Edge3<T> &&) noexcept = default;

        constexpr Edge3(const Vec3<T>& v1, const Vec3<T>& v2) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value) :
            vertices{ v1, v2 }
        {
        }

        constexpr Edge3<T> translated(const Vec3<T>& displacement) const
        {
            return Edge3<T>(vertices[0] + displacement, vertices[1] + displacement);
        }

        Edge3<T>& translate(const Vec3<T>& displacement)
        {
            vertices[0] += displacement;
            vertices[1] += displacement;

            return *this;
        }

        template <typename T2>
        constexpr explicit operator Edge3<T2>() const
        {
            return Edge3<T2>(static_cast<Vec3<T2>>(vertices[0]), static_cast<Vec3<T2>>(vertices[1]));
        }

        constexpr T length() const
        {
            return vertices[0].distance(vertices[1]);
        }

        constexpr T lengthSquared() const
        {
            return vertices[0].distanceSquared(vertices[1]);
        }

        constexpr Vec3<T> centerOfMass() const
        {
            return (vertices[0] + vertices[1]) * T(0.5);
        }
    };

    using Edge3D = Edge3<double>;
    using Edge3F = Edge3<float>;

    template <typename T>
    constexpr bool operator== (const Edge3<T>& lhs, const Edge3<T>& rhs)
    {
        return std::tie(lhs.vertices[0], lhs.vertices[1]) == std::tie(rhs.vertices[0], rhs.vertices[1]);
    }
    template <typename T>
    constexpr bool operator!= (const Edge3<T>& lhs, const Edge3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

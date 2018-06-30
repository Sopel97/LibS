#pragma once

#include "Vec2.h"

#include <array>
#include <type_traits>
#include <tuple>

namespace ls
{
    template <typename T>
    struct Edge2
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        std::array<Vec2<T>, 2u> vertices;

        constexpr Edge2() noexcept = default;

        constexpr Edge2(const Edge2<T>&) = default;
        constexpr Edge2(Edge2<T>&&) noexcept = default;

        Edge2<T>& operator=(const Edge2<T>&) = default;
        Edge2<T>& operator=(Edge2<T> &&) noexcept = default;

        constexpr Edge2(const Vec2<T>& v1, const Vec2<T>& v2) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
            vertices{ v1, v2 }
        {
        }

        constexpr Edge2<T> translated(const Vec2<T>& displacement) const
        {
            return Edge2<T>(vertices[0] + displacement, vertices[1] + displacement);
        }

        Edge2<T>& translate(const Vec2<T>& displacement)
        {
            vertices[0] += displacement;
            vertices[1] += displacement;

            return *this;
        }

        template <typename T2>
        explicit operator Edge2<T2>() const
        {
            return Edge2<T2>(static_cast<Vec2<T2>>(vertices[0]), static_cast<Vec2<T2>>(vertices[1]));
        }

        constexpr T length() const
        {
            return vertices[0].distance(vertices[1]);
        }

        constexpr T lengthSquared() const
        {
            return vertices[0].distanceSquared(vertices[1]);
        }

        constexpr Vec2<T> centerOfMass() const
        {
            return (vertices[0] + vertices[1]) * T(0.5);
        }
    };

    using Edge2D = Edge2<double>;
    using Edge2F = Edge2<float>;

    template <typename T>
    constexpr bool operator== (const Edge2<T>& lhs, const Edge2<T>& rhs)
    {
        return std::tie(lhs.vertices[0], lhs.vertices[1]) == std::tie(rhs.vertices[0], rhs.vertices[1]);
    }
    template <typename T>
    constexpr bool operator!= (const Edge2<T>& lhs, const Edge2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

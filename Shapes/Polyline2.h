#pragma once

#include "Vec2.h"

#include <vector>
#include <type_traits>

namespace ls
{
    template <typename T>
    struct Polyline2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        std::vector<Vec2<T>> vertices;

        Polyline2() noexcept = default;

        Polyline2(const Polyline2<T>&) = default;
        Polyline2(Polyline2<T>&&) noexcept = default;

        Polyline2<T>& operator=(const Polyline2<T>&) = default;
        Polyline2<T>& operator=(Polyline2<T> &&) noexcept = default;

        explicit Polyline2(const std::vector<Vec2<T>>& vertices) :
            vertices(vertices)
        {
        }

        explicit Polyline2(std::vector<Vec2<T>>&& vertices) noexcept :
            vertices(std::move(vertices))
        {
        }

        template <typename InputIt>
        Polyline2(InputIt begin, InputIt end) :
            vertices(begin, end)
        {

        }

        Polyline2<T> translated(const Vec2<T>& displacement) const
        {
            Polyline2 newPolyline(*this);
            newPolyline.translate(displacement);
            return newPolyline;
        }
        Polyline2<T>& translate(const Vec2<T>& displacement)
        {
            for (Vec2<T>& vertex : vertices)
            {
                vertex += displacement;
            }
            return *this;
        }

        template <typename T2>
        explicit operator Polyline2<T2>() const
        {
            std::vector<Vec2<T2>> poly;
            poly.reserve(vertices.size());
            for (const auto& v : vertices)
            {
                poly.emplace_back(static_cast<Vec2<T2>>(v));
            }
            return Polyline2<T>(std::move(poly));
        }
    };

    using Polyline2D = Polyline2<double>;
    using Polyline2F = Polyline2<float>;
}

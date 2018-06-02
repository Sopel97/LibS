#pragma once

#include "Vec2.h"

#include <vector>
#include <initializer_list>
#include <utility>
#include <algorithm>
#include <type_traits>

namespace ls
{
    template <typename T>
    struct ConvexPolygon2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        std::vector<Vec2<T>> vertices;

        ConvexPolygon2() noexcept = default;

        ConvexPolygon2(const ConvexPolygon2<T>&) = default;
        ConvexPolygon2(ConvexPolygon2<T>&&) noexcept = default;

        ConvexPolygon2<T>& operator=(const ConvexPolygon2<T>&) = default;
        ConvexPolygon2<T>& operator=(ConvexPolygon2<T> &&) noexcept = default;

        explicit ConvexPolygon2(const std::vector<Vec2<T>>& _vertices) :
            vertices(vertices)
        {
        }

        explicit ConvexPolygon2(std::vector<Vec2<T>>&& _vertices) noexcept :
            vertices(std::move(vertices))
        {
        }

        template <typename InputIt>
        ConvexPolygon2(InputIt begin, InputIt end) :
            vertices(begin, end)
        {

        }

        ConvexPolygon2<T> translated(const Vec2<T>& displacement) const
        {
            ConvexPolygon2 newPolygon(*this);
            newPolygon.translate(displacement);
            return newPolygon;
        }
        ConvexPolygon2<T>& translate(const Vec2<T>& displacement)
        {
            for (Vec2<T>& vertex : vertices)
            {
                vertex += displacement;
            }
            return *this;
        }

        template <typename T2>
        explicit operator ConvexPolygon2<T2>() const
        {
            std::vector<Vec2<T2>> poly;
            poly.reserve(vertices.size());
            for (const auto& v : vertices)
            {
                poly.emplace_back(static_cast<Vec2<T2>>(v));
            }
            return ConvexPolygon2<T>(std::move(poly));
        }

        Vec2<T> centerOfMass() const
        {
            const size_t numVertices = vertices.size();

            T xsum{ 0 };
            T ysum{ 0 };
            T area{ 0 };
            for (size_t i = 0; i < numVertices; ++i)
            {
                const Vec2<T>& p0 = vertices[i];
                const Vec2<T>& p1 = vertices[(i + 1) % numVertices];

                T areaSum = (p0.x * p1.y) - (p1.x * p0.y);

                xsum += (p0.x + p1.x) * areaSum;
                ysum += (p0.y + p1.y) * areaSum;

                area += areaSum;
            }

            T centMassX = xsum / (area * T(3));
            T centMassY = ysum / (area * T(3));

            return Vec2<T>(centMassX, centMassY);
        }

        T signedArea() const
        {
            const size_t numVertices = vertices.size();

            T area = 0;
            for (size_t i = 0; i < numVertices; ++i)
            {
                const Vec2<T>& p0 = vertices[i];
                const Vec2<T>& p1 = vertices[(i + 1) % numVertices];

                area += (p0.x * p1.y) - (p1.x * p0.y);
            }
            return area * T(0.5);
        }

        T area() const
        {
            return std::abs(signedArea());
        }
    };

    using ConvexPolygon2D = ConvexPolygon2<double>;
    using ConvexPolygon2F = ConvexPolygon2<float>;
}

#pragma once

#include "LibS/Shapes/Vec2.h"

#include <array>
#include <type_traits>

namespace ls
{
    template <typename T>
    struct CubicBezierCurve2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

        using VectorType = Vec2<T>;
        static constexpr int numControlPoints = 4;

        std::array<Vec2<T>, numControlPoints> controlPoints;

        CubicBezierCurve2() = default;
        CubicBezierCurve2(const CubicBezierCurve2&) = default;
        CubicBezierCurve2(CubicBezierCurve2&&) = default;
        CubicBezierCurve2& operator=(const CubicBezierCurve2&) = default;
        CubicBezierCurve2& operator=(CubicBezierCurve2&&) = default;
        ~CubicBezierCurve2() = default;

        CubicBezierCurve2(const Vec2<T>& p0, const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3) :
            controlPoints({ p0, p1, p2, p3 })
        {

        }

        explicit CubicBezierCurve2(const std::array<Vec2<T>, numControlPoints>& points) :
            controlPoints(points)
        {

        }

        Vec2<T> at(const T& t) const
        {
            const T u = static_cast<T>(1) - t;
            const T t2 = t * t;
            const T t3 = t2 * t;
            const T u2 = u * u;
            const T u3 = u2 * u;

            return
                u3 * controlPoints[0]
                + static_cast<T>(3) * u2 * t * controlPoints[1]
                + static_cast<T>(3) * u * t2 * controlPoints[2]
                + t3 * controlPoints[3];
        }
    };

    using CubicBezierCurve2F = CubicBezierCurve2<float>;
    using CubicBezierCurve2D = CubicBezierCurve2<double>;
}
#pragma once

#include "Fwd.h"

#include "LibS/Shapes/Vec2.h"

#include <array>
#include <type_traits>
#include <utility>

namespace ls
{
    template <typename T>
    struct BezierCurve2<T, 2>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

        using VectorType = Vec2<T>;
        static constexpr int order = 2;
        static constexpr int numControlPoints = order + 1;

        std::array<Vec2<T>, numControlPoints> controlPoints;

        BezierCurve2() = default;
        BezierCurve2(const BezierCurve2&) = default;
        BezierCurve2(BezierCurve2&&) = default;
        BezierCurve2& operator=(const BezierCurve2&) = default;
        BezierCurve2& operator=(BezierCurve2&&) = default;
        ~BezierCurve2() = default;

        BezierCurve2(const BezierCurve2<T, order - 1>& linearCurve) :
            BezierCurve2(linearCurve.elevate())
        {
        }

        BezierCurve2(const Vec2<T>& p0, const Vec2<T>& p1, const Vec2<T>& p2) :
            controlPoints({ p0, p1, p2 })
        {

        }

        explicit BezierCurve2(const std::array<Vec2<T>, numControlPoints>& points) :
            controlPoints(points)
        {

        }

        void translate(const Vec2<T>& displacement)
        {
            for (auto& cp : controlPoints)
            {
                cp += displacement;
            }
        }

        Vec2<T> at(const T& t) const
        {
            const T u = static_cast<T>(1) - t;
            const T t2 = t * t;
            const T u2 = u * u;

            return
                u2 * controlPoints[0]
                + static_cast<T>(2) * u * t * controlPoints[1]
                + t2 * controlPoints[2];
        }

        BezierCurve2<T, order> left(const T& z) const
        {
            const T z2 = z * z;
            const T w = static_cast<T>(1) - z;
            const T w2 = w * w;

            const Vec2<T> p0 = controlPoints[0];

            const Vec2<T> p1 =
                z * controlPoints[1]
                + w * controlPoints[0];

            const Vec2<T> p2 =
                z2 * controlPoints[2]
                + static_cast<T>(2) * z * w * controlPoints[1]
                + w2 * controlPoints[0];

            return BezierCurve2<T, order>(p0, p1, p2);
        }

        BezierCurve2<T, order> right(const T& z) const
        {
            const T z2 = z * z;
            const T w = static_cast<T>(1) - z;
            const T w2 = w * w;

            const Vec2<T> p2 =
                z2 * controlPoints[2]
                + static_cast<T>(2) * z * w * controlPoints[1]
                + w2 * controlPoints[0];

            const Vec2<T> p3 =
                z * controlPoints[2]
                + w * controlPoints[1];

            const Vec2<T> p4 = controlPoints[2];

            return BezierCurve2<T, order>(p2, p3, p4);
        }

        BezierCurve2<T, order> subcurve(const T& min, const T& max) const
        {
            return right(min).left((max - min) / (static_cast<T>(1) - min));
        }

        std::pair<BezierCurve2<T, order>, BezierCurve2<T, order>> split(const T& z) const
        {
            const T z2 = z * z;
            const T w = static_cast<T>(1) - z;
            const T w2 = w * w;

            const Vec2<T> p0 = controlPoints[0];

            const Vec2<T> p1 =
                z * controlPoints[1]
                + w * controlPoints[0];

            const Vec2<T> p2 =
                z2 * controlPoints[2]
                + static_cast<T>(2) * z * w * controlPoints[1]
                + w2 * controlPoints[0];

            const Vec2<T> p3 =
                z * controlPoints[2]
                + w * controlPoints[1];

            const Vec2<T> p4 = controlPoints[2];

            return {
                BezierCurve2<T, order>(p0, p1, p2),
                BezierCurve2<T, order>(p2, p3, p4)
            };
        }

        BezierCurve2<T, order - 1> derivative() const
        {
            return BezierCurve2<T, order - 1>(
                static_cast<T>(2) * (controlPoints[1] - controlPoints[0]),
                static_cast<T>(2) * (controlPoints[2] - controlPoints[1])
            );
        }

        BezierCurve2<T, order + 1> elevate() const
        {
            return BezierCurve2<T, order + 1>(
                controlPoints[0],
                static_cast<T>(1.0 / 3.0) * controlPoints[0] + static_cast<T>(2.0 / 3.0) * controlPoints[1],
                static_cast<T>(2.0 / 3.0) * controlPoints[1] + static_cast<T>(1.0 / 3.0) * controlPoints[2],
                controlPoints[2]
            );
        }
    };

    using QuadraticBezierCurve2F = QuadraticBezierCurve2<float>;
    using QuadraticBezierCurve2D = QuadraticBezierCurve2<double>;
}
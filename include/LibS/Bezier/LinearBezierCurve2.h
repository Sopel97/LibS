#pragma once

#include "Fwd.h"

#include "LibS/Shapes/Vec2.h"

#include <array>
#include <type_traits>
#include <utility>

namespace ls
{
    template <typename T>
    struct BezierCurve2<T, 1>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

        using VectorType = Vec2<T>;
        static constexpr int order = 1;
        static constexpr int numControlPoints = order + 1;

        std::array<Vec2<T>, numControlPoints> controlPoints;

        BezierCurve2() = default;
        BezierCurve2(const BezierCurve2&) = default;
        BezierCurve2(BezierCurve2&&) = default;
        BezierCurve2& operator=(const BezierCurve2&) = default;
        BezierCurve2& operator=(BezierCurve2&&) = default;
        ~BezierCurve2() = default;

        BezierCurve2(const Vec2<T>& p0, const Vec2<T>& p1) :
            controlPoints({ p0, p1 })
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

            return
                u * controlPoints[0]
                + t * controlPoints[1];
        }

        BezierCurve2<T, order> left(const T& z) const
        {
            const T w = static_cast<T>(1) - z;

            const Vec2<T> p0 = controlPoints[0];

            const Vec2<T> p1 =
                z * controlPoints[1]
                + w * controlPoints[0];

            return BezierCurve2<T, order>(p0, p1);
        }

        BezierCurve2<T, order> right(const T& z) const
        {
            const T w = static_cast<T>(1) - z;

            const Vec2<T> p1 =
                z * controlPoints[1]
                + w * controlPoints[0];

            const Vec2<T> p2 = controlPoints[1];

            return BezierCurve2<T, order>(p1, p2);
        }

        BezierCurve2<T, order> subcurve(const T& min, const T& max) const
        {
            return right(min).left((max - min) / (static_cast<T>(1) - min));
        }

        std::pair<BezierCurve2<T, order>, BezierCurve2<T, order>> split(const T& z) const
        {
            const T w = static_cast<T>(1) - z;

            const Vec2<T> p0 = controlPoints[0];

            const Vec2<T> p1 =
                z * controlPoints[1]
                + w * controlPoints[0];

            const Vec2<T> p2 = controlPoints[1];

            return {
                BezierCurve2<T, order>(p0, p1),
                BezierCurve2<T, order>(p1, p2)
            };
        }

        BezierCurve2<T, order> aligned() const
        {
            const Vec2<T>& p0 = controlPoints[0];
            const Vec2<T>& p1 = controlPoints[order];

            const auto a = -((p1 - p0).angle());

            const T s = a.sin();
            const T c = a.cos();

            BezierCurve2<T, order> alignedCurve(*this);

            for (auto& p : alignedCurve.controlPoints)
            {
                const Vec2<T> p0p = p - p0;
                p.x = p0p.x * c - p0p.y * s;
                p.y = p0p.x * s + p0p.y * c;
            }

            return alignedCurve;
        }

        BezierCurve2<T, order + 1> elevate() const
        {
            return BezierCurve2<T, order + 1>(
                controlPoints[0],
                static_cast<T>(0.5) * (controlPoints[0] + controlPoints[1]),
                controlPoints[1]
            );
        }
    };

    using QuadraticBezierCurve2F = QuadraticBezierCurve2<float>;
    using QuadraticBezierCurve2D = QuadraticBezierCurve2<double>;
}
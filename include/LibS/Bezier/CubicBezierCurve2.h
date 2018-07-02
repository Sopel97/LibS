#pragma once

#include "Fwd.h"

#include "LibS/Shapes/Vec2.h"
#include "LibS/Shapes/Angle2.h"

#include <array>
#include <type_traits>
#include <utility>

namespace ls
{
    template <typename T>
    struct BezierCurve2<T, 3>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

        using VectorType = Vec2<T>;
        static constexpr int order = 3;
        static constexpr int numControlPoints = order + 1;

        std::array<Vec2<T>, numControlPoints> controlPoints;

        BezierCurve2() = default;
        BezierCurve2(const BezierCurve2&) = default;
        BezierCurve2(BezierCurve2&&) = default;
        BezierCurve2& operator=(const BezierCurve2&) = default;
        BezierCurve2& operator=(BezierCurve2&&) = default;
        ~BezierCurve2() = default;

        template <int OtherOrderV, typename Enable = std::enable_if_t<(OrderV > OtherOrderV)>>
        BezierCurve2(const BezierCurve2<T, OtherOrderV>& linearCurve) :
            BezierCurve2(linearCurve.elevate())
        {
        }

        BezierCurve2(const Vec2<T>& p0, const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3) :
            controlPoints({ p0, p1, p2, p3 })
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
            const T t3 = t2 * t;
            const T u2 = u * u;
            const T u3 = u2 * u;

            return
                u3 * controlPoints[0]
                + static_cast<T>(3) * u2 * t * controlPoints[1]
                + static_cast<T>(3) * u * t2 * controlPoints[2]
                + t3 * controlPoints[3];
        }

        BezierCurve2<T, order> left(const T& z) const
        {
            const T z2 = z * z;
            const T z3 = z2 * z;
            const T w = static_cast<T>(1) - z;
            const T w2 = w * w;
            const T w3 = w2 * w;

            const Vec2<T> p0 = controlPoints[0];

            const Vec2<T> p1 =
                z * controlPoints[1]
                + w * controlPoints[0];

            const Vec2<T> p2 =
                z2 * controlPoints[2]
                + static_cast<T>(2) * z * w * controlPoints[1]
                + w2 * controlPoints[0];

            const Vec2<T> p3 =
                z3 * controlPoints[3]
                + static_cast<T>(3) * z2 * w * controlPoints[2]
                + static_cast<T>(3) * z * w2 * controlPoints[1]
                + w3 * controlPoints[0];

            return BezierCurve2<T, order>(p0, p1, p2, p3);
        }

        BezierCurve2<T, order> right(const T& z) const
        {
            const T z2 = z * z;
            const T z3 = z2 * z;
            const T w = static_cast<T>(1) - z;
            const T w2 = w * w;
            const T w3 = w2 * w;

            const Vec2<T> p3 =
                z3 * controlPoints[3]
                + static_cast<T>(3) * z2 * w * controlPoints[2]
                + static_cast<T>(3) * z * w2 * controlPoints[1]
                + w3 * controlPoints[0];

            const Vec2<T> p4 =
                z2 * controlPoints[3]
                + static_cast<T>(2) * z * w * controlPoints[2]
                + w2 * controlPoints[1];

            const Vec2<T> p5 =
                z * controlPoints[3]
                + w * controlPoints[2];

            const Vec2<T> p6 = controlPoints[3];

            return BezierCurve2<T, order>(p3, p4, p5, p6);
        }

        BezierCurve2<T, order> subcurve(const T& min, const T& max) const
        {
            return right(min).left((max - min) / (static_cast<T>(1) - min));
        }

        std::pair<BezierCurve2<T, order>, BezierCurve2<T, order>> split(const T& z) const
        {
            const T z2 = z * z;
            const T z3 = z2 * z;
            const T w = static_cast<T>(1) - z;
            const T w2 = w * w;
            const T w3 = w2 * w;

            const Vec2<T> p0 = controlPoints[0];

            const Vec2<T> p1 =
                z * controlPoints[1]
                + w * controlPoints[0];

            const Vec2<T> p2 =
                z2 * controlPoints[2]
                + static_cast<T>(2) * z * w * controlPoints[1]
                + w2 * controlPoints[0];

            const Vec2<T> p3 =
                z3 * controlPoints[3]
                + static_cast<T>(3) * z2 * w * controlPoints[2]
                + static_cast<T>(3) * z * w2 * controlPoints[1]
                + w3 * controlPoints[0];

            const Vec2<T> p4 =
                z2 * controlPoints[3]
                + static_cast<T>(2) * z * w * controlPoints[2]
                + w2 * controlPoints[1];

            const Vec2<T> p5 =
                z * controlPoints[3]
                + w * controlPoints[2];

            const Vec2<T> p6 = controlPoints[3];

            return {
                BezierCurve2<T, order>(p0, p1, p2, p3),
                BezierCurve2<T, order>(p3, p4, p5, p6)
            };
        }

        BezierCurve2<T, order> aligned() const
        {
            const Vec2<T> p0 = controlPoints[0];
            const Vec2<T> p1 = controlPoints[order];

            const auto n = (p1 - p0).normalized();

            // -angle so mirrored by x axis
            const T sin = -n.y;
            const T cos = n.x;

            BezierCurve2<T, order> alignedCurve(*this);

            for (auto& p : alignedCurve.controlPoints)
            {
                const Vec2<T> p0p = p - p0;
                // p.x = p0p.dot({ cos, -sin });
                // p.y = p0p.dot({ sin, cos });
                p.x = p0p.x * cos - p0p.y * sin;
                p.y = p0p.x * sin + p0p.y * cos;
            }

            return alignedCurve;
        }

        Vec2<T> canonicalFormFreePoint(const T& unit = static_cast<T>(1)) const
        {
            const Vec2<T> p0 = controlPoints[0];
            const Vec2<T> p1 = controlPoints[1];
            const Vec2<T> p2 = controlPoints[2];
            const Vec2<T> p3 = controlPoints[3];

            const Vec2<T> p0p1 = p1 - p0;
            const Vec2<T> p0p2 = p2 - p0;
            const Vec2<T> p0p3 = p3 - p0;

            const T p0p1r = p0p1.x / p0p1.y;

            const T xnum = p0p3.x - (p0p1r * p0p3.y);
            const T xden = p0p2.x - (p0p1r * p0p2.y);

            const T x = xnum / xden;

            const T y = (p0p3.y / p0p1.y) + x * (static_cast<T>(1) - (p0p2.y / p0p1.y));

            return { unit * x, unit * y };
        }

        BezierCurve2<T, order> canonical(const T& unit = static_cast<T>(1)) const
        {
            return BezierCurve2<T, order>(
                { 0, 0 },
                { 0, unit },
                { unit, unit },
                canonicalFormFreePoint(unit)
                );
        }

        BezierCurve2<T, order - 1> derivative() const
        {
            return BezierCurve2<T, order - 1>(
                static_cast<T>(3) * (controlPoints[1] - controlPoints[0]),
                static_cast<T>(3) * (controlPoints[2] - controlPoints[1]),
                static_cast<T>(3) * (controlPoints[3] - controlPoints[2])
                );
        }

        BezierCurve2<T, order + 1> elevate() const
        {
            return BezierCurve2<T, order + 1>(
                controlPoints[0],
                static_cast<T>(1.0 / 4.0) * controlPoints[0] + static_cast<T>(3.0 / 4.0) * controlPoints[1],
                static_cast<T>(2.0 / 4.0) * controlPoints[1] + static_cast<T>(2.0 / 4.0) * controlPoints[2],
                static_cast<T>(3.0 / 4.0) * controlPoints[2] + static_cast<T>(1.0 / 4.0) * controlPoints[3],
                controlPoints[3]
                );
        }
    };

    using CubicBezierCurve2F = CubicBezierCurve2<float>;
    using CubicBezierCurve2D = CubicBezierCurve2<double>;
}
#pragma once

#include "Fwd.h"
#include "Detail.h"

#include "LibS/Shapes/Vec2.h"

#include <array>
#include <type_traits>
#include <utility>
#include <cmath>

namespace ls
{
    template <typename T, int OrderV>
    struct BezierCurve2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
        static_assert(OrderV > 0, "Order must be positive");

        using VectorType = Vec2<T>;
        static constexpr int order = OrderV;
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

        template <typename... Ts, typename Enable = std::enable_if_t<(sizeof...(Ts) == OrderV + 1)>>
        BezierCurve2(Ts&&... ts) :
            controlPoints({ ts... })
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
            if constexpr (order == 1)
            {
                const T u = static_cast<T>(1) - t;

                return
                    u * controlPoints[0]
                    + t * controlPoints[1];
            }
            else if constexpr (order == 2)
            {
                const T u = static_cast<T>(1) - t;
                const T t2 = t * t;
                const T u2 = u * u;

                return
                    u2 * controlPoints[0]
                    + static_cast<T>(2) * u * t * controlPoints[1]
                    + t2 * controlPoints[2];
            }
            else if constexpr (order == 3)
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
            else
            {
                using std::pow;
                using std::abs;

                const T u = static_cast<T>(1) - t;

                Vec2<T> sum = Vec2<T>::zero();

                // since we iterate powers backwards we have to avoid division by 0
                if (abs(u) < detail::bezierCurveArgEps<T>)
                {
                    sum = controlPoints[order];
                }
                else
                {
                    auto bc = detail::PascalTriangleLine<T, order>{}.begin();
                    //T tpow = static_cast<T>(1);
                    T upow = pow(u, order);
                    T tu = /* tpow * */ upow;
                    const T tu_ratio = t / u;
                    for (int k = 0; k < numControlPoints; ++k)
                    {
                        // sum += (*bc) * pow(u, order - k) * pow(t, k) * controlPoints[k];
                        sum += (*bc) * tu * controlPoints[k];
                        ++bc;
                        tu *= tu_ratio;
                    }
                }
                return sum;
            }
        }

        BezierCurve2<T, order> left(const T& z) const
        {
            if constexpr (order == 1)
            {
                const T w = static_cast<T>(1) - z;

                const Vec2<T> p0 = controlPoints[0];

                const Vec2<T> p1 =
                    z * controlPoints[1]
                    + w * controlPoints[0];

                return BezierCurve2<T, order>(p0, p1);
            }
            else if constexpr (order == 2)
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
            else if constexpr (order == 3)
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
            else
            {
                using std::pow;
                using std::abs;

                const T w = static_cast<T>(1) - z;

                // since we iterate powers backwards by using division we have to avoid division by 0
                if (abs(w) < detail::bezierCurveArgEps<T>)
                {
                    return *this;
                }
                else
                {
                    std::array<Vec2<T>, numControlPoints> left;

                    left[0] = controlPoints[0];

                    for (int i = 1; i <= order; ++i)
                    {
                        Vec2<T> sum = Vec2<T>::zero();

                        auto bc = detail::PascalTriangleLine<T>{ i }.begin();
                        //T zpow = static_cast<T>(1);
                        T wpow = pow(w, i);
                        T zw = /* zpow * */ wpow;
                        const T zw_ratio = z / w;
                        for (int j = 0; j <= i; ++j)
                        {
                            // sum += (*bc) * pow(w, i - j) * pow(z, j) * controlPoints[j];
                            sum += (*bc) * zw * controlPoints[j];
                            ++bc;
                            zw *= zw_ratio;
                        }

                        left[i] = sum;
                    }

                    return BezierCurve2<T, order>(left);
                }
            }
        }

        BezierCurve2<T, order> right(const T& z) const
        {
            if constexpr (order == 1)
            {
                const T w = static_cast<T>(1) - z;

                const Vec2<T> p1 =
                    z * controlPoints[1]
                    + w * controlPoints[0];

                const Vec2<T> p2 = controlPoints[1];

                return BezierCurve2<T, order>(p1, p2);
            }
            else if constexpr (order == 2)
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
            else if constexpr (order == 3)
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
            else
            {
                using std::pow;

                // since we iterate powers backwards by using division we have to avoid division by 0
                if (abs(z) < detail::bezierCurveArgEps<T>)
                {
                    return *this;
                }
                else
                {
                    const T w = static_cast<T>(1) - z;

                    std::array<Vec2<T>, numControlPoints> right;

                    for (int i = 1; i <= order; ++i)
                    {
                        Vec2<T> sum = Vec2<T>::zero();

                        auto bc = detail::PascalTriangleLine<T>{ i }.begin();
                        //T wpow = static_cast<T>(1);
                        T zpow = pow(z, i);
                        T wz = /* wpow * */ zpow;
                        const T wz_ratio = w / z;
                        for (int j = 0; j <= i; ++j)
                        {
                            // sum += (*bc) * pow(z, i - j) * pow(w, j) * controlPoints[order - j];
                            sum += (*bc) * wz * controlPoints[order - j];
                            ++bc;
                            wz *= wz_ratio;
                        }

                        right[order - i] = sum;
                    }

                    right[order] = controlPoints[order];

                    return BezierCurve2<T, order>(right);
                }
            }
        }

        BezierCurve2<T, order> subcurve(const T& min, const T& max) const
        {
            return right(min).left((max - min) / (static_cast<T>(1) - min));
        }

        std::pair<BezierCurve2<T, order>, BezierCurve2<T, order>> split(const T& z) const
        {
            if constexpr (order == 1)
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
            else if constexpr (order == 2)
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
            else if constexpr (order == 3)
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
            else
            {
                using std::pow;

                const T w = static_cast<T>(1) - z;

                return std::make_pair(left(z), right(z)); //they calculate one point twice but the overhead is negligable
            }
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

        template <int OrdV = OrderV, typename EnableT = std::enable_if_t<(OrdV > 1)>>
        BezierCurve2<T, order - 1> derivative() const
        {
            if constexpr (order == 2)
            {
                return BezierCurve2<T, order - 1>(
                    static_cast<T>(2) * (controlPoints[1] - controlPoints[0]),
                    static_cast<T>(2) * (controlPoints[2] - controlPoints[1])
                    );
            }
            else if constexpr (order == 3)
            {
                return BezierCurve2<T, order - 1>(
                    static_cast<T>(3) * (controlPoints[1] - controlPoints[0]),
                    static_cast<T>(3) * (controlPoints[2] - controlPoints[1]),
                    static_cast<T>(3) * (controlPoints[3] - controlPoints[2])
                    );
            }
            else
            {
                std::array<Vec2<T>, order> derivativeControlPoints;

                for (int i = 0; i < order; ++i)
                {
                    derivativeControlPoints[i] = static_cast<T>(order) * (controlPoints[i + 1] - controlPoints[i]);
                }

                return BezierCurve2<T, order - 1>(derivativeControlPoints);
            }
        }

        BezierCurve2<T, order + 1> elevate() const
        {
            if constexpr (order == 1)
            {
                return BezierCurve2<T, order + 1>(
                    controlPoints[0],
                    static_cast<T>(0.5) * (controlPoints[0] + controlPoints[1]),
                    controlPoints[1]
                    );
            }
            else if constexpr (order == 2)
            {
                return BezierCurve2<T, order + 1>(
                    controlPoints[0],
                    static_cast<T>(1.0 / 3.0) * controlPoints[0] + static_cast<T>(2.0 / 3.0) * controlPoints[1],
                    static_cast<T>(2.0 / 3.0) * controlPoints[1] + static_cast<T>(1.0 / 3.0) * controlPoints[2],
                    controlPoints[2]
                    );
            }
            else if constexpr (order == 3)
            {
                return BezierCurve2<T, order + 1>(
                    controlPoints[0],
                    static_cast<T>(1.0 / 4.0) * controlPoints[0] + static_cast<T>(3.0 / 4.0) * controlPoints[1],
                    static_cast<T>(2.0 / 4.0) * controlPoints[1] + static_cast<T>(2.0 / 4.0) * controlPoints[2],
                    static_cast<T>(3.0 / 4.0) * controlPoints[2] + static_cast<T>(1.0 / 4.0) * controlPoints[3],
                    controlPoints[3]
                    );
            }
            else
            {
                std::array<Vec2<T>, numControlPoints + 1> elevatedControlPoints;

                elevatedControlPoints[0] = controlPoints[0];

                for (int i = 1; i <= order; ++i)
                {
                    elevatedControlPoints[i] =
                        (
                            static_cast<T>(i) * controlPoints[i - 1]
                            + static_cast<T>(order + 1 - i) * controlPoints[i]
                            ) / static_cast<T>(order + 1);
                }

                elevatedControlPoints[order + 1] = controlPoints[order];

                return BezierCurve2<T, order + 1>(elevatedControlPoints);
            }
        }

        template <int OrdV = OrderV, typename EnableT = std::enable_if_t<OrdV == 3>>
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

        template <int OrdV = OrderV, typename EnableT = std::enable_if_t<OrdV == 3>>
        BezierCurve2<T, order> canonical(const T& unit = static_cast<T>(1)) const
        {
            return BezierCurve2<T, order>(
                Vec2<T>{ 0, 0 },
                Vec2<T>{ 0, unit },
                Vec2<T>{ unit, unit },
                canonicalFormFreePoint(unit)
                );
        }

        template <typename IntegratorT>
        T length(const T& min, const T& max, IntegratorT&& integrator) const
        {
            if constexpr(order == 1)
            {
                return (max - min) * controlPoints[0].distance(controlPoints[1]);
            }
            else
            {
                auto d = derivative();

                return integrator(
                    min, max,
                    [&d](const T& t) {
                        using std::sqrt;

                        const auto p = d.at(t);
                        return std::sqrt(p.x*p.x + p.y*p.y);
                    }
                );
            }
        }

        template <typename IntegratorT>
        T lengthAt(const T& t, IntegratorT&& integrator) const
        {
            return length(static_cast<T>(0), t, std::forward<IntegratorT>(integrator));
        }

        template <typename IntegratorT>
        T length(IntegratorT&& integrator) const
        {
            return length(static_cast<T>(0), static_cast<T>(1), std::forward<IntegratorT>(integrator));
        }
    };

    template <int OrderV>
    using BezierCurve2F = BezierCurve2<float, OrderV>;

    template <int OrderV>
    using BezierCurve2D = BezierCurve2<double, OrderV>;
}
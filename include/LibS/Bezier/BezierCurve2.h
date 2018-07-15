#pragma once

#include "Fwd.h"
#include "CommonDetail.h"

#include "LibS/Shapes/Vec2.h"
#include "LibS/Shapes/Edge2.h"

#include "LibS/Common.h"

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

        using ValueType = T;
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

        BezierCurve2<T, order> translated(const Vec2<T>& displacement)
        {
            auto cpy = BezierCurve2<T, order>(*this);
            cpy.translate(displacement);
            return cpy;
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

        template <typename IntegratorT>
        T lengthToParam(const T& wantedLength, int numMaxIters, IntegratorT&& integrator, const ValueType& eps = static_cast<ValueType>(0.0001)) const
        {
            // same algorithm as in EquidistnatBezierCurveParams

            const auto curveDerivative = derivative();

            ValueType tLeft = static_cast<T>(0);
            ValueType tRight = static_cast<T>(1);
            ValueType sLeft = static_cast<T>(0);
            ValueType sRight = curveDerivative.antiderivativeLength(std::forward<IntegratorT>(integrator));

            ValueType tCurrent = tLeft + (tRight - tLeft) * ((wantedLength - sLeft) / (sRight - sLeft)); // linearly interpolate t by length
            ValueType sCurrent = curveDerivative.antiderivativeLengthAt(tCurrent, integrator);

            for (int j = 0; j < numMaxIters && abs(wantedLength - sCurrent) > eps; ++j)
            {
                if (sCurrent < wantedLength)
                {
                    //left
                    tLeft = std::move(tCurrent);
                    sLeft = std::move(sCurrent);
                }
                else
                {
                    //right
                    tRight = std::move(tCurrent);
                    sRight = std::move(sCurrent);
                }

                tCurrent = tLeft + (tRight - tLeft) * ((wantedLength - sLeft) / (sRight - sLeft)); // linearly interpolate t by length
                sCurrent = curveDerivative.antiderivativeLengthAt(tCurrent, integrator);
            }

            return tCurrent;
        }

        template <typename IntegratorT>
        T paramToLength(const T& t, IntegratorT&& integrator) const
        {
            return lengthAt(t, std::forward<IntegratorT>(integrator));
        }

        template <typename IntegratorT>
        Vec2<T> atLength(const T& s, int numMaxIters, IntegratorT&& integrator, const ValueType& eps = static_cast<ValueType>(0.0001)) const
        {
            return at(lengthToParam(s, numMaxIters, std::forward<IntegratorT>(integrator), eps));
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

        BezierCurve2<T, order> aligned(const ls::Edge2<T>& line) const
        {
            const Vec2<T> p0 = line.vertices[0];
            const Vec2<T> p1 = line.vertices[1];

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

        BezierCurve2<T, order> aligned() const
        {
            return aligned({ controlPoints[0], controlPoints[order] });
        }

        template <T Vec2<T>::*axis>
        std::vector<T> axisAlignedExtrema() const
        {
            if constexpr (order == 1)
            {
                return {};
            }
            else if constexpr (order == 2)
            {
                // derivative in the form of ax + b
                const auto d = derivative();
                // p0 * (1-t) + p1 * t
                // p0 - t*p0 + t*p1
                // t*(p1 - p0) + p0
                // x = -b/a    <-- root
                const T a = (d.controlPoints[1].*axis - d.controlPoints[0].*axis);
                const T b = d.controlPoints[0].*axis;

                const T t = -b / a;
                if (t >= static_cast<T>(0) && t <= static_cast<T>(1))
                {
                    return { t };
                }

                return {};
            }
            else if constexpr (order == 3)
            {
                constexpr T deltaEps = static_cast<T>(0.000001);

                // derivative in the form of ax + b
                const auto d = derivative();
                // p0 * (1-t)^2 + 2 * p1 * (1-t)t + p2 * t^2
                // p0 * (1 - 2t + t^2) + 2 * p1 * (t - t^2) + p2 * t^2
                // t^2 * (p0 - 2 * p1 + p2) + t * 2(p1 - p0) + p0
                // a = p0 - 2 * p1 + p2
                // b = 2(p1 - p0)
                // c = p0
                const T a = (d.controlPoints[0].*axis - static_cast<T>(2) * d.controlPoints[1].*axis + d.controlPoints[2].*axis);
                const T b = static_cast<T>(2) * (d.controlPoints[1].*axis - d.controlPoints[0].*axis);
                const T c = d.controlPoints[0].*axis;

                const T delta = (b*b) - (static_cast<T>(4)*a*c);
                
                if (almostZero(delta, deltaEps))
                {
                    const T t = -b / (static_cast<T>(2) * a);

                    if (t >= static_cast<T>(0) && t <= static_cast<T>(1))
                    {
                        return { t };
                    }
                }
                else if (delta > static_cast<T>(0))
                {
                    using std::sqrt;
                    const T sqrtDelta = sqrt(delta);

                    const T t1 = (-b + sqrtDelta) / (static_cast<T>(2) * a);
                    const T t2 = (-b - sqrtDelta) / (static_cast<T>(2) * a);

                    std::vector<T> result;
                    if (t1 >= static_cast<T>(0) && t1 <= static_cast<T>(1))
                    {
                        result.emplace_back(t1);
                    }
                    if (t2 >= static_cast<T>(0) && t2 <= static_cast<T>(1))
                    {
                        result.emplace_back(t2);
                    }
                    return result;
                }

                return {};
            }
            else
            {
                static_assert(false);
            }
        }

        std::vector<T> extremaX() const
        {
            return axisAlignedExtrema<&Vec2<T>::x>();
        }

        std::vector<T> extremaY() const
        {
            return axisAlignedExtrema<&Vec2<T>::y>();
        }

        std::pair<std::vector<T>, std::vector<T>> extrema() const
        {
            return std::make_pair(
                axisAlignedExtrema<&Vec2<T>::x>(),
                axisAlignedExtrema<&Vec2<T>::y>()
            );
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
        T antiderivativeLength(const T& min, const T& max, IntegratorT&& integrator) const
        {
            return integrator(
                min, max,
                [this](const T& t) {
                    using std::sqrt;

                    const auto p = this->at(t);

                    return sqrt(p.x*p.x + p.y*p.y);
                }
            );
        }

        template <typename IntegratorT>
        T antiderivativeLengthAt(const T& t, IntegratorT&& integrator) const
        {
            return antiderivativeLength(static_cast<T>(0), t, std::forward<IntegratorT>(integrator));
        }

        template <typename IntegratorT>
        T antiderivativeLength(IntegratorT&& integrator) const
        {
            return antiderivativeLength(static_cast<T>(0), static_cast<T>(1), std::forward<IntegratorT>(integrator));
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
                return derivative().antiderivativeLength(min, max, std::forward<IntegratorT>(integrator));
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
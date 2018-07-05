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
        static_assert(OrderV > 3, "For orders less than 4 use specializations");

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

        template <typename... Ts, typename Enable = std::enable_if_t<(sizeof...(Ts) > 1)>>
        BezierCurve2(Ts&&... ts) :
            controlPoints({ ts... })
        {
            static_assert(sizeof...(Ts) == numControlPoints, "Number of control points doesn't match number of parameters");
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

        BezierCurve2<T, order> left(const T& z) const
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

        BezierCurve2<T, order> right(const T& z) const
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

        BezierCurve2<T, order> subcurve(const T& min, const T& max) const
        {
            return right(min).left((max - min) / (static_cast<T>(1) - min));
        }

        std::pair<BezierCurve2<T, order>, BezierCurve2<T, order>> split(const T& z) const
        {
            using std::pow;

            const T w = static_cast<T>(1) - z;

            return std::make_pair(left(z), right(z)); //they calculate one point twice but the overhead is negligable
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

        BezierCurve2<T, order - 1> derivative() const
        {
            std::array<Vec2<T>, order> derivativeControlPoints;
            
            for (int i = 0; i < order; ++i)
            {
                derivativeControlPoints[i] = static_cast<T>(order) * (controlPoints[i + 1] - controlPoints[i]);
            }

            return BezierCurve2<T, order - 1>(derivativeControlPoints);
        }

        BezierCurve2<T, order + 1> elevate() const
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
    };

    template <int OrderV>
    using BezierCurve2F = BezierCurve2<float, OrderV>;

    template <int OrderV>
    using BezierCurve2D = BezierCurve2<double, OrderV>;
}
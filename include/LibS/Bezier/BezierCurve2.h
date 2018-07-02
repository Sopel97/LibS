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

            const T u = static_cast<T>(1) - t;

            Vec2<T> sum = Vec2<T>::zero();

            for (int k = 0; k < numControlPoints; ++k)
            {
                sum += static_cast<T>(detail::BinomialCoefficients::value(order, k)) * pow(u, order - k) * pow(t, k) * controlPoints[k];
            }

            return sum;
        }

        BezierCurve2<T, order> left(const T& z) const
        {
            using std::pow;

            const T w = static_cast<T>(1) - z;

            std::array<Vec2<T>, numControlPoints> left;

            left[0] = controlPoints[0];

            for (int i = 1; i <= order; ++i)
            {
                Vec2<T> sum = Vec2<T>::zero();

                for (int j = 0; j <= i; ++j)
                {
                    sum += static_cast<T>(detail::BinomialCoefficients::value(i, j)) * pow(w, i - j) * pow(z, j) * controlPoints[j];
                }

                left[i] = sum;
            }

            return BezierCurve2<T, order>(left);
        }

        BezierCurve2<T, order> right(const T& z) const
        {
            using std::pow;

            const T w = static_cast<T>(1) - z;

            std::array<Vec2<T>, numControlPoints> right;

            for (int i = 1; i <= order; ++i)
            {
                Vec2<T> sum = Vec2<T>::zero();

                for (int j = 0; j <= i; ++j)
                {
                    sum += static_cast<T>(detail::BinomialCoefficients::value(i, j)) * pow(w, j) * pow(z, i - j) * controlPoints[order - j];
                }

                right[order - i] = sum;
            }

            right[order] = controlPoints[order];

            return BezierCurve2<T, order>(right);
        }

        BezierCurve2<T, order> subcurve(const T& min, const T& max) const
        {
            return right(min).left((max - min) / (static_cast<T>(1) - min));
        }

        std::pair<BezierCurve2<T, order>, BezierCurve2<T, order>> split(const T& z) const
        {
            using std::pow;

            const T w = static_cast<T>(1) - z;

            std::array<Vec2<T>, numControlPoints> left;
            std::array<Vec2<T>, numControlPoints> right;

            left[0] = controlPoints[0];

            for (int i = 1; i <= order; ++i)
            {
                Vec2<T> sum = Vec2<T>::zero();

                for (int j = 0; j <= i; ++j)
                {
                    sum += static_cast<T>(detail::BinomialCoefficients::value(i, j)) * pow(w, i - j) * pow(z, j) * controlPoints[j];
                }

                left[i] = sum;
            }

            right[0] = left[order];

            for (int i = 1; i < order; ++i)
            {
                Vec2<T> sum = Vec2<T>::zero();

                for (int j = 0; j <= i; ++j)
                {
                    sum += static_cast<T>(detail::BinomialCoefficients::value(i, j)) * pow(w, j) * pow(z, i - j) * controlPoints[order - j];
                }

                right[order - i] = sum;
            }

            right[order] = controlPoints[order];

            return {
                BezierCurve2<T, order>(left),
                BezierCurve2<T, order>(right)
            };
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

            for (int i = 0; i < order; ++i)
            {
                elevatedControlPoints[i + 1] = 
                    (static_cast<T>(i) / static_cast<T>(order)) * controlPoints[i] 
                    + (static_cast<T>(order - i) / static_cast<T>(order)) * controlPoints[i + 1];
            }

            elevatedControlPoints[numControlPoints] = controlPoints[order];


            return BezierCurve2<T, order + 1>(elevatedControlPoints);
        }
    };

    template <int OrderV>
    using BezierCurve2F = BezierCurve2<float, OrderV>;

    template <int OrderV>
    using BezierCurve2D = BezierCurve2<double, OrderV>;
}
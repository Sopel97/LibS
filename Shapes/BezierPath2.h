#pragma once

#include "Vec2.h"
#include "../BinomialCoefficients.h"

#include <vector>
#include <type_traits>

namespace ls
{
    template <typename T>
    struct BezierPath2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        std::vector<Vec2<T>> controlPoints;

        BezierPath2() noexcept = default;

        BezierPath2(const BezierPath2<T>&) = default;
        BezierPath2(BezierPath2<T>&&) noexcept = default;

        BezierPath2<T>& operator=(const BezierPath2<T>&) = default;
        BezierPath2<T>& operator=(BezierPath2<T>&&) noexcept = default;

        BezierPath2(const std::vector<Vec2<T>>& controlPoints) :
            controlPoints(controlPoints)
        {

        }

        BezierPath2(std::vector<Vec2<T>>&& controlPoints) noexcept :
            controlPoints(std::move(controlPoints))
        {

        }

        template<typename InputIt>
        BezierPath2(InputIt begin, InputIt end) :
            controlPoints(begin, end)
        {

        }

        Vec2<T> evaluate(const T& t) const
        {
            Vec2<T> point(T(0), T(0));
            const int numberOfControlPoints = static_cast<int>(controlPoints.size());
            const int numberOfSubCurves = (numberOfControlPoints - 1) / 3;

            const T tt = t * static_cast<T>(numberOfSubCurves);
            const int tti = floorToInt(tt);
            int subCurveAtT = tti;

            if (subCurveAtT < 0) subCurveAtT = 0;
            if (subCurveAtT >= numberOfSubCurves) subCurveAtT = numberOfSubCurves - 1;

            const int firstControlPoint = subCurveAtT * 3;
            const Vec2<T>& controlPoint0 = controlPoints[firstControlPoint];
            const Vec2<T>& controlPoint1 = controlPoints[firstControlPoint + 1];
            const Vec2<T>& controlPoint2 = controlPoints[firstControlPoint + 2];
            const Vec2<T>& controlPoint3 = controlPoints[firstControlPoint + 3];

            const T localT = tt - static_cast<T>(tti);
            point += controlPoint0 * static_cast<T>(std::pow(T(1) - localT, 3));
            point += controlPoint1 * static_cast<T>(std::pow(T(1) - localT, 2)) * 3;
            point += controlPoint2 * static_cast<T>(std::pow(T(1) - localT, 2)) * 3;
            point += controlPoint3 * static_cast<T>(std::pow(T(1) - localT, 3));

            return point;
        }

        std::vector<Vec2<T>> evaluateAll(int numberOfSamples) const
        {
            return evaluateRange(T(0), T(1), numberOfSamples);
        }

        std::vector<Vec2<T>> evaluateRange(const T& rangeMin, const T& rangeMax, int numberOfSamples) const
        {
            std::vector<Vec2<T>> result;
            result.reserve(numberOfSamples);

            const T step = (rangeMax - rangeMin) / static_cast<T>(numberOfSamples - 1);

            T t = rangeMin;
            int i = 0;
            while (i < numberOfSamples)
            {
                result.emplace_back(evaluate(t));

                t += step;
                ++i;
            }

            return result;
        }

        std::vector<Vec2<T>> evaluateAll(const T& step) const
        {
            return evaluateRange(T(0), T(1), step);
        }

        std::vector<Vec2<T>> evaluateRange(const T& rangeMin, const T& rangeMax, const T& step) const
        {
            std::vector<Vec2<T>> result;
            result.reserve(static_cast<int>((rangeMax - rangeMin) / step) + 1);

            T t = rangeMin;
            while (t <= rangeMax)
            {
                result.emplace_back(evaluate(t));

                t += step;
            }

            return result;
        }
    };

    using BezierPath2D = BezierPath2<double>;
    using BezierPath2F = BezierPath2<float>;
}

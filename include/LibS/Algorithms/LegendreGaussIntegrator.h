#pragma once

#include <array>
#include <type_traits>

namespace ls
{
    namespace detail
    {
        template <typename T, int N>
        struct LegendreGaussWeights;

        template <typename T, int N>
        struct LegendreGaussAbscissae;

        template <typename T>
        struct LegendreGaussWeights<T, 8>
        {
            static constexpr std::array<T, 8> values = {
                0.3626837833783620,
                0.3626837833783620,
                0.3137066458778873,
                0.3137066458778873,
                0.2223810344533745,
                0.2223810344533745,
                0.1012285362903763,
                0.1012285362903763
            };
        };

        template <typename T>
        struct LegendreGaussAbscissae<T, 8>
        {
            static constexpr std::array<T, 8> values = {
                -0.1834346424956498,
                0.1834346424956498,
                -0.5255324099163290,
                0.5255324099163290,
                -0.7966664774136267,
                0.7966664774136267,
                -0.9602898564975363,
                0.9602898564975363
            };
        };

        template <typename T>
        struct LegendreGaussWeights<T, 16>
        {
            static constexpr std::array<T, 16> values = {};
        };

        template <typename T>
        struct LegendreGaussAbscissae<T, 16>
        {
            static constexpr std::array<T, 16> values = {};
        };

        template <typename T>
        struct LegendreGaussWeights<T, 24>
        {
            static constexpr std::array<T, 24> values = {};
        };

        template <typename T>
        struct LegendreGaussAbscissae<T, 24>
        {
            static constexpr std::array<T, 24> values = {};
        };

        template <typename T>
        struct LegendreGaussWeights<T, 32>
        {
            static constexpr std::array<T, 32> values = {};
        };

        template <typename T>
        struct LegendreGaussAbscissae<T, 32>
        {
            static constexpr std::array<T, 32> values = {};
        };
    }

    template <typename T, int N>
    struct LegendreGaussIntegrator
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

        using WeightsType = detail::LegendreGaussWeights<T, N>;
        using AbscissaeType = detail::LegendreGaussAbscissae<T, N>;

        template <typename FuncT>
        T operator()(const T& left, const T& right, FuncT&& func) const {
            constexpr auto& weights = WeightsType::values;
            constexpr auto& abscissae = AbscissaeType::values;

            T result = static_cast<T>(0);
            const T halfIntervalLength = (right - left) / static_cast<T>(2);
            const T intervalMiddle = (right + left) / static_cast<T>(2);
            // [left, right] -> [-1, 1]
            const T intervalStretchFactor = halfIntervalLength;

            for (int i = 0; i < N; ++i)
            {
                // t must be in range [-1, 1]
                const T x = (abscissae[i] * halfIntervalLength) + intervalMiddle;

                result += weights[i] * func(x);
            }

            return halfIntervalLength * result;
        }
    };

    using LegendreGaussIntegrator8D = LegendreGaussIntegrator<double, 8>;
    using LegendreGaussIntegrator16D = LegendreGaussIntegrator<double, 16>;
    using LegendreGaussIntegrator24D = LegendreGaussIntegrator<double, 24>;
    using LegendreGaussIntegrator32D = LegendreGaussIntegrator<double, 32>;

    using LegendreGaussIntegrator8F = LegendreGaussIntegrator<float, 8>;
    using LegendreGaussIntegrator16F = LegendreGaussIntegrator<float, 16>;
    using LegendreGaussIntegrator24F = LegendreGaussIntegrator<float, 24>;
    using LegendreGaussIntegrator32F = LegendreGaussIntegrator<float, 32>;
}

#pragma once

#include <type_traits>
#include <random>

namespace ls
{
    namespace detail
    {
        template <typename T, typename EnableT = void>
        struct UniformDistribution;

        template <typename T>
        struct UniformDistribution<T, std::enable_if_t<std::is_integral_v<T>>>
        {
            using Type = std::uniform_int_distribution<T>;
        };

        template <typename T>
        struct UniformDistribution<T, std::enable_if_t<std::is_floating_point_v<T>>>
        {
            using Type = std::uniform_real_distribution<T>;
        };

        template <typename T>
        using UniformDistributionT = typename UniformDistribution<T>::Type;
    }
}
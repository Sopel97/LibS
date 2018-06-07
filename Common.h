#pragma once

#include <cstddef>

namespace ls
{
    template <typename T>
    constexpr bool almostZero(const T& val, const T& tolerance)
    {
        using std::abs;

        return abs(val) <= tolerance;
    }
    template <typename T>
    constexpr bool almostEqual(const T& lhs, const T& rhs, const T& tolerance)
    {
        using std::abs;

        return abs(lhs - rhs) <= tolerance;
    }

    template <typename T, typename U>
    constexpr T lerp(const T& lhs, const T& rhs, const U& t)
    {
        return lhs + (rhs - lhs) * t;
    }
    template <typename T, typename U>
    constexpr T nlerp(const T& lhs, const T& rhs, const U& t)
    {
        return (lhs + (rhs - lhs) * t).normalized();
    }
    // evaluates v%p but converts negative results so that they are not symetric to positive ones
    template <typename IntType>
    IntType periodic(IntType v, IntType p)
    {
        v %= p;
        IntType vp = (v >= 0) ? v : v + p;
        return vp;
    }

    template <typename FloatT, typename IntT = int>
    IntT floorToInt(const FloatT& value)
    {
        const int i = static_cast<IntT>(value);
        return i - (value < FloatT(0.0));
    }
}

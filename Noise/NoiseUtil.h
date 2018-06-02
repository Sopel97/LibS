#pragma once

#include "../Fwd.h"

#include "../Shapes/Vec2.h"
#include "../Shapes/Vec3.h"
#include "../Shapes/Vec4.h"

#include <memory>
#include <algorithm>

namespace ls
{
    struct OriginalPerlinPermTable
    {
    private:
        static constexpr unsigned char m_defaultPerm[256] = {
            151,160,137,91,90,15,
            131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
            190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
            88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
            77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
            102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
            135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
            5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
            223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
            129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
            251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
            49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
            138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
        };

    public:
        OriginalPerlinPermTable() = default;

        uint32_t operator()(uint32_t x) const
        {
            return m_defaultPerm[x & 0xFF];
        }
    };

    template <int SizeV, typename IntT>
    struct PermTable
    {
        static_assert(std::is_unsigned<IntT>::value, "Must be an unsigned type");
    private:
        std::unique_ptr<IntT[]> m_perm;

        PermTable(std::unique_ptr<IntT[]>&& perm) :
            m_perm(std::move(perm))
        {

        }

    public:
        template <typename RngT>
        static PermTable<SizeV, IntT> shuffled(RngT&& rng)
        {
            std::unique_ptr<IntT[]> perm = std::make_unique<IntT[]>(SizeV);
            std::iota(perm.get(), perm.get() + SizeV, 0);
            std::shuffle(perm.get(), perm.get() + SizeV, rng);
            return PermTable<SizeV, IntT>(std::move(perm));
        }

        IntT operator()(IntT x) const
        {
            return m_perm[x % N];
        }
    };

    template <typename T, int DimV>
    struct ValueDerivativePair
    {
        using VectorType = VecT<T, DimV>;

        T value;
        VectorType derivative;

        ValueDerivativePair() = default;
        ValueDerivativePair(const T& v) noexcept(std::is_nothrow_copy_constructible<T>::value) :
            value(v),
            derivative(v)
        {

        }
        ValueDerivativePair(const T& v, VecT<T, DimV> d) noexcept(std::is_nothrow_copy_constructible<VectorType>::value) :
            value(v),
            derivative(d)
        {

        }

        ValueDerivativePair(const ValueDerivativePair<T, DimV>& other) = default;
        ValueDerivativePair(ValueDerivativePair<T, DimV>&& other) = default;

        ValueDerivativePair<T, DimV>& operator=(const ValueDerivativePair<T, DimV>& other) = default;
        ValueDerivativePair<T, DimV>& operator=(ValueDerivativePair<T, DimV>&& other) = default;

        ValueDerivativePair<T, DimV>& operator+=(const ValueDerivativePair<T, DimV>& rhs)
        {
            value += rhs.value;
            derivative += rhs.derivative;
            return *this;
        }
        ValueDerivativePair<T, DimV>& operator-=(const ValueDerivativePair<T, DimV>& rhs)
        {
            value -= rhs.value;
            derivative -= rhs.derivative;
            return *this;
        }
        ValueDerivativePair<T, DimV>& operator*=(const T& rhs)
        {
            value *= rhs;
            derivative *= rhs;
            return *this;
        }
        ValueDerivativePair<T, DimV>& operator/=(const T& rhs)
        {
            value /= rhs;
            derivative /= rhs;
            return *this;
        }
    };

    template <typename T, int DimV>
    ValueDerivativePair<T, DimV> operator+(const ValueDerivativePair<T, DimV>& lhs, const ValueDerivativePair<T, DimV>& rhs)
    {
        ValueDerivativePair<T, DimV> result(lhs);
        result += rhs;
        return result;
    }
    template <typename T, int DimV>
    ValueDerivativePair<T, DimV> operator-(const ValueDerivativePair<T, DimV>& lhs, const ValueDerivativePair<T, DimV>& rhs)
    {
        ValueDerivativePair<T, DimV> result(lhs);
        result -= rhs;
        return result;
    }
    template <typename T, int DimV>
    ValueDerivativePair<T, DimV> operator*(const ValueDerivativePair<T, DimV>& lhs, const T& rhs)
    {
        ValueDerivativePair<T, DimV> result(lhs);
        result *= rhs;
        return result;
    }
    template <typename T, int DimV>
    ValueDerivativePair<T, DimV> operator/(const ValueDerivativePair<T, DimV>& lhs, const T& rhs)
    {
        ValueDerivativePair<T, DimV> result(lhs);
        result /= rhs;
        return result;
    }
    template <typename T, int DimV>
    ValueDerivativePair<T, DimV> operator-(const ValueDerivativePair<T, DimV>& arg)
    {
        ValueDerivativePair<T, DimV> result(arg);
        result *= T(-1);
        return result;
    }

    namespace detail
    {
        template <typename T, typename U>
        inline T scaleResult(const T& val, const U& lowerBound, const U& upperBound)
        {
            return (val + T(1)) * U(0.5) * (upperBound - lowerBound) + T(lowerBound);
        }

        template <typename T>
        inline T lerp(const T& a, const T& b, const T& t)
        {
            return a + t * (b - a);
        }

        template <typename T>
        inline T fade(const T& t)
        {
            return (t * t * t * (t * (t * T(6) - T(15)) + T(10)));
        }
    }
}
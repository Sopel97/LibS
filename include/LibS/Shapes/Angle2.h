#pragma once

#include "LibS/MathConstants.h"
#include "LibS/Common.h"

#include <type_traits>
#include <cmath>

namespace ls
{
    namespace detail
    {
        template <typename T>
        constexpr T radiansToDegrees(const T& radians)
        {
            return radians * (T(180.0) / pi<T>);
        }
        template <typename T>
        constexpr T degreesToRadians(const T& degrees)
        {
            return degrees * (pi<T> / T(180.0));
        }
    }
    template <typename T>
    struct Angle2 //directed angle 
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;

        constexpr static Angle2<T> radians(const T& rad) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value)
        {
            return Angle2<T>(rad);
        }

        constexpr static Angle2<T> degrees(const T& deg) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value)
        {
            return Angle2<T>(detail::degreesToRadians(deg));
        }

        constexpr static const Angle2<T>& fullTurn()
        {
            constexpr static Angle2<T> angle(T(2.0) * pi<T>);
            return angle;
        }

        constexpr Angle2() noexcept = default;

        constexpr Angle2(const Angle2<T>&) = default;
        constexpr Angle2(Angle2<T>&&) noexcept = default;

        constexpr Angle2<T>& operator=(const Angle2<T>&) = default;
        constexpr Angle2<T>& operator=(Angle2<T> &&) noexcept = default;

        constexpr Angle2<T>& operator+=(const Angle2<T>& rhs)
        {
            m_radians += rhs.m_radians;
            return *this;
        }

        constexpr Angle2<T>& operator-=(const Angle2<T>& rhs)
        {
            m_radians -= rhs.m_radians;
            return *this;
        }

        constexpr Angle2<T>& operator*=(const T& rhs)
        {
            m_radians *= rhs;
            return *this;
        }

        constexpr Angle2<T>& operator/=(const T& rhs)
        {
            m_radians /= rhs;
            return *this;
        }

        constexpr void normalize()
        {
            // cannot use fmod because it requires the result to be non-negative
            const int fullTurns = floorToInt(m_radians / fullTurn().m_radians);
            m_radians -= fullTurns * fullTurn().m_radians;
        }

        constexpr Angle2<T> normalized() const // [0, 2pi)
        {
            Angle2<T> result(*this);

            result.normalize();

            return result;
        }

        template <typename T2>
        constexpr explicit operator Angle2<T2>() const
        {
            return Angle2<T2>(static_cast<T2>(m_radians));
        }

        constexpr T sin() const
        {
            using std::sin;

            return static_cast<T>(sin(m_radians));
        }

        constexpr T cos() const
        {
            using std::cos;

            return static_cast<T>(cos(m_radians));
        }

        constexpr T tan() const
        {
            using std::tan;

            return static_cast<T>(tan(m_radians));
        }

        constexpr T cot() const
        {
            using std::tan;

            return T(1) / static_cast<T>(tan(m_radians));
        }

        constexpr static Angle2<T> asin(const T& val)
        {
            using std::asin;

            return Angle2<T>(static_cast<T>(asin(val)));
        }

        constexpr static Angle2<T> acos(const T& val)
        {
            using std::acos;

            return Angle2<T>(static_cast<T>(acos(val)));
        }

        constexpr static Angle2<T> atan(const T& val)
        {
            using std::atan;

            return Angle2<T>(static_cast<T>(atan(val)));
        }

        constexpr static Angle2<T> acot(const T& val)
        {
            using std::atan;

            return Angle2<T>(static_cast<T>(atan(T(1))) / val);
        }

        constexpr T sinh() const
        {
            using std::sinh;

            return static_cast<T>(sinh(m_radians));
        }

        constexpr T cosh() const
        {
            using std::cosh;

            return static_cast<T>(cosh(m_radians));
        }

        constexpr T tanh() const
        {
            using std::tanh;

            return static_cast<T>(tanh(m_radians));
        }

        constexpr T coth() const
        {
            using std::tanh;

            return T(1) / static_cast<T>(tanh(m_radians));
        }

        constexpr static Angle2<T> asinh(const T& val)
        {
            using std::atanh;

            return Angle2<T>(static_cast<T>(atanh(val)));
        }

        constexpr static Angle2<T> acosh(const T& val)
        {
            using std::acosh;

            return Angle2<T>(static_cast<T>(acosh(val)));
        }

        constexpr static Angle2<T> atanh(const T& val)
        {
            using std::atanh;

            return Angle2<T>(static_cast<T>(atanh(val)));
        }

        constexpr static Angle2<T> acoth(const T& val)
        {
            using std::atanh;

            return Angle2<T>(static_cast<T>(atanh(T(1) / val)));
        }

        constexpr static Angle2<T> atan2(const T& y, const T& x)
        {
            using std::atan2;

            return Angle2<T>(static_cast<T>(std::atan2(y, x)));
        }

        constexpr T radians() const
        {
            return m_radians;
        }

        constexpr T degrees() const
        {
            return detail::radiansToDegrees(m_radians);
        }

    protected:
    private:
        T m_radians;

        constexpr Angle2(const T& rad) noexcept(std::is_nothrow_constructible<T, T>::value) :
            m_radians(rad)
        {
        }
    };

    using Angle2D = Angle2<double>;
    using Angle2F = Angle2<float>;

    template <typename T>
    constexpr bool operator==(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return lhs.radians() == rhs.radians();
    }

    template <typename T>
    constexpr bool operator!=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename T>
    constexpr bool operator<(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return lhs.radians() < rhs.radians();
    }

    template <typename T>
    constexpr bool operator<=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename T>
    constexpr bool operator>(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return rhs.radians() < lhs.radians();
    }

    template <typename T>
    constexpr bool operator>=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(lhs < rhs);
    }

    template <typename T>
    constexpr Angle2<T> operator+(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return Angle2<T>::radians(lhs.radians() + rhs.radians());
    }

    template <typename T>
    constexpr Angle2<T> operator-(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return Angle2<T>::radians(lhs.radians() - rhs.radians());
    }

    template <typename T>
    constexpr Angle2<T> operator*(const Angle2<T>& lhs, const T& rhs)
    {
        return Angle2<T>::radians(lhs.radians() * rhs);
    }

    template <typename T>
    constexpr Angle2<T> operator/(const Angle2<T>& lhs, const T& rhs)
    {
        return Angle2<T>::radians(lhs.radians() / rhs);
    }

    template <typename T>
    constexpr Angle2<T> operator-(const Angle2<T>& angle)
    {
        return Angle2<T>::radians(-angle.radians());
    }

}

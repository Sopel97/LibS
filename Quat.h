#pragma once

#include "Shapes/Angle2.h"
#include "Shapes/Vec3.h"

#include "EulerAngles.h"
#include "AxisAngle.h"

namespace ls
{
    namespace detail
    {
        template <typename T>
        struct quat_align
        {
            static constexpr size_t value = alignof(T);
        };

        template <>
        struct quat_align<float>
        {
            static constexpr size_t value = 16;
        };
    }

    template <typename T>
    struct alignas(detail::quat_align<T>::value) Quat
    {
    public:
        T w;
        T x;
        T y;
        T z;

        static constexpr const Quat<T>& identity()
        {
            static const Quat<T> q{};
            return q;
        }

        constexpr Quat() noexcept :
            w(1),
            x(0),
            y(0),
            z(0)
        {

        }

        constexpr Quat(const T& w, const T& x, const T& y, const T& z) :
            w(w),
            x(x),
            y(y),
            z(z)
        {

        }

        constexpr Quat(EulerAngles<T> a)
        {
            a.pitch *= T(0.5);
            a.yaw *= T(0.5);
            a.roll *= T(0.5);

            const T cp = a.pitch.cos();
            const T cy = a.yaw.cos();
            const T cr = a.roll.cos();

            const T sp = a.pitch.sin();
            const T sy = a.yaw.sin();
            const T sr = a.roll.sin();

            z = cp * cy * sr - sp * sy * cr;
            w = cp * cy * cr + sp * sy * sr;
            x = sp * cy * cr - cp * sy * sr;
            y = cp * sy * cr + sp * cy * sr;
        }

        constexpr Quat(const AxisAngle<T>& a)
        {
            const Angle2<T> halfAngle = a.angle() * T(0.5);
            
            const T s = halfAngle.sin();
            const T c = halfAngle.cos();

            w = c;
            x = a.axis().x * s;
            y = a.axis().y * s;
            z = a.axis().z * s;
        }

        /*
        Quat(const Matrix<T, 3, 3>& m)
        {
            using std::sqrt;

            const T fourXSquaredMinus1 = m[0][0] - m[1][1] - m[2][2];
            const T fourYSquaredMinus1 = m[1][1] - m[0][0] - m[2][2];
            const T fourZSquaredMinus1 = m[2][2] - m[0][0] - m[1][1];
            const T fourWSquaredMinus1 = m[0][0] + m[1][1] + m[2][2];

            int biggestIndex = 0;
            T fourBiggestSquaredMinus1 = fourWSquaredMinus1;
            if (fourXSquaredMinus1 > fourBiggestSquaredMinus1)
            {
                fourBiggestSquaredMinus1 = fourXSquaredMinus1;
                biggestIndex = 1;
            }
            if (fourYSquaredMinus1 > fourBiggestSquaredMinus1)
            {
                fourBiggestSquaredMinus1 = fourYSquaredMinus1;
                biggestIndex = 2;
            }
            if (fourZSquaredMinus1 > fourBiggestSquaredMinus1)
            {
                fourBiggestSquaredMinus1 = fourZSquaredMinus1;
                biggestIndex = 3;
            }

            const T biggestVal = sqrt(fourBiggestSquaredMinus1 + T(1)) * T(0.5);
            const T mult = T(0.25) / biggestVal;

            switch (biggestIndex)
            {
            case 0:
                w = biggestVal;
                x = (m[1][2] - m[2][1]) * mult;
                y = (m[2][0] - m[0][2]) * mult;
                z = (m[0][1] - m[1][0]) * mult;
                break;
            case 1:
                w = (m[1][2] - m[2][1]) * mult;
                x = biggestVal;
                y = (m[0][1] + m[1][0]) * mult;
                z = (m[2][0] + m[0][2]) * mult;
                break;
            case 2:
                w = (m[2][0] - m[0][2]) * mult;
                x = (m[0][1] + m[1][0]) * mult;
                y = biggestVal;
                z = (m[1][2] + m[2][1]) * mult;
                break;
            default: // 3
                w = (m[0][1] - m[1][0]) * mult;
                x = (m[2][0] + m[0][2]) * mult;
                y = (m[1][2] + m[2][1]) * mult;
                z = biggestVal;
                break;
            }
        }
        */

        constexpr Quat(const Quat<T>&) = default;
        constexpr Quat(Quat<T>&&) noexcept = default;
        constexpr Quat<T>& operator=(const Quat<T>&) = default;
        constexpr Quat<T>& operator=(Quat<T>&&) noexcept = default;

        constexpr Quat<T>& operator+=(const Quat<T>& rhs)
        {
            w += rhs.w;
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;

            return *this;
        }

        constexpr Quat<T>& operator-=(const Quat<T>& rhs)
        {
            w -= rhs.w;
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;

            return *this;
        }

        constexpr Quat<T>& operator*=(const Quat<T>& rhs)
        {
            const T newW = w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z;
            const T newX = w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y;
            const T newY = w*rhs.y - x*rhs.z + y*rhs.w + z*rhs.x;
            const T newZ = w*rhs.z + x*rhs.y - y*rhs.x + z*rhs.w;

            w = newW;
            x = newX;
            y = newY;
            z = newZ;

            return *this;
        }

        constexpr Quat<T>& operator*=(const T& rhs)
        {
            w *= rhs;
            x *= rhs;
            y *= rhs;
            z *= rhs;

            return *this;
        }

        constexpr Quat<T>& operator/=(const T& rhs)
        {
            w /= rhs;
            x /= rhs;
            y /= rhs;
            z /= rhs;

            return *this;
        }

        Quat<T> normalized() const
        {
            Quat<T> result(*this);
            result.normalize();
            return result;
        }

        void normalize()
        {
            const T scale = T(1) / length();
            (*this) *= scale;
        }

        constexpr Quat<T> conjugate() const
        {
            return Quat<T>(w, -x, -y, -z);
        }

        void invert()
        {
            const T scale = T(1) / lengthSquared();

            (*this) = conjugate() * scale;
        }

        Quat<T> inverse() const
        {
            Quat<T> result(*this);
            result.invert();
            return result;
        }

        constexpr T dot(const Quat<T>& rhs) const
        {
            return w*rhs.w + x*rhs.x + y*rhs.y + z*rhs.z;
        }

        // http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
        constexpr Vec3<T> apply(const Vec3<T>& v) const
        {
            const Vec3<T> q(x, y, z);
            const Vec3<T> t = T(2) * q.cross(v);
            const Vec3<T> r = v + (w * t) + q.cross(t);
            return r;
        }

        constexpr T lengthSquared() const
        {
            return w*w + x*x + y*y + z*z;
        }

        T length() const
        {
            using std::sqrt;

            return sqrt(lengthSquared());
        }

        // e^q
        Quat<T> exp() const
        {
            static constexpr T eps = T(0.00001);

            using std::sqrt;
            using std::sin;
            using std::cos;

            const T vlen = sqrt(x*x + y*y + z*z);

            if (vlen > eps)
            {
                const T s = sin(vlen);
                const T c = cos(vlen);
                const T scale = s / vlen;
                return Quat<T>(
                    c,
                    scale * x,
                    scale * y,
                    scale * z
                );
            }
            else 
            {
                return Quat<T>(T(1), T(0), T(0), T(0));
            }
        }

        Quat<T> log() const
        {
            using std::atan2;
            using std::log;

            static constexpr T eps = T(0.00001);

            const T vlen = sqrt(x*x + y*y + z*z);

            if (vlen > eps)
            {
                const T t = atan2(vlen, w) / vlen;
                const T len = vlen * vlen + w * w;
                return Quat<T>(T(0.5) * log(len), t * x, t * y, t * z);
            }
            else
            {
                if (w > T(0))
                {
                    return Quat<T>(log(w), T(0), T(0), T(0));
                }
                else if (w < T(0))
                {
                    return Quat<T>(log(-w), pi<T>, T(0), T(0));
                }
                else
                {
                    return Quat<T>(std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity(), std::numeric_limits<T>::infinity());
                }
            }
        }


    private:
    };

    using QuatD = Quat<double>;
    using QuatF = Quat<float>;

    template <typename T>
    Quat<T> operator+(const Quat<T>& lhs, const Quat<T>& rhs)
    {
        Quat<T> result(lhs);
        return (result += rhs);
    }

    template <typename T>
    Quat<T> operator-(const Quat<T>& lhs, const Quat<T>& rhs)
    {
        Quat<T> result(lhs);
        return (result -= rhs);
    }

    template <typename T>
    Quat<T> operator*(const Quat<T>& lhs, const Quat<T>& rhs)
    {
        Quat<T> result(lhs);
        return (result *= rhs);
    }

    template <typename T>
    Quat<T> operator*(const Quat<T>& lhs, const T& rhs)
    {
        Quat<T> result(lhs);
        return (result *= rhs);
    }

    template <typename T>
    Quat<T> operator/(const Quat<T>& lhs, const T& rhs)
    {
        Quat<T> result(lhs);
        return (result /= rhs);
    }

    template <typename T>
    Vec3<T> operator*(const Vec3<T>& lhs, const Quat<T>& rhs)
    {
        return rhs.apply(lhs);
    }


    // https://en.wikipedia.org/wiki/Slerp
    template <typename T>
    constexpr Quat<T> slerp(const Quat<T>& lhs, const Quat<T>& rhs, const T& t)
    {
        using std::sin;
        using std::cos;
        using std::acos;

        static constexpr T eps = T(0.01);

        Quat<T> q = rhs;
        T cosTheta = std::clamp(lhs.dot(rhs), T(-1.0), T(1.0));

        // take shorter path
        if (cosTheta < T(0))
        {
            q = -rhs;
            cosTheta = -cosTheta;
        }

        // avoid division by 0, use lerp if sin(theta) ~ 0
        if (cosTheta > T(1) - eps)
        {
            return nlerp(lhs, rhs, t);
        }
        else
        {
            //const T theta = acos(cosTheta);
            // return (sin((T(1) - t) * theta) * lhs + sin(t * theta) * q) / sin(theta);

            const T theta = acos(cosTheta) * t;
            q -= lhs*cosTheta;
            q.normalize();
            // { lhs, q } now an orthonormal basis
            return lhs*cos(theta) + q*sin(theta);
        }
    }
}

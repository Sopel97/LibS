#pragma once

#include "Quat.h"
#include "Shapes/Vec3.h"

namespace ls
{
    // QST(A) = Q(A)*S(A)*P*-Q(A) + T(A)
    template <typename T>
    struct Transform3
    {
    public:
        Quat<T> rotation;
        Vec3<T> scale;
        Vec3<T> translation;

        static constexpr Transform3<T> withRotation(const Quat<T>& q)
        {
            return Transform3<T>(q, Vec3<T>::one(), Vec3<T>::zero());
        }
        static constexpr Transform3<T> withRotation(const Quat<T>& q, const Vec3<T>& origin)
        {
            return withTranslation(origin) * withRotation(q) * withTranslation(-origin);
        }
        static constexpr Transform3<T> withScale(const T& s)
        {
            return Transform3<T>(Quat<T>::identity(), Vec3<T>(s, s, s), Vec3<T>::zero());
        }
        static constexpr Transform3<T> withScale(const Vec3<T>& s)
        {
            return Transform3<T>(Quat<T>::identity(), s, Vec3<T>::zero());
        }
        static constexpr Transform3<T> withTranslation(const Vec3<T>& v)
        {
            return Transform3<T>(Quat<T>::identity(), Vec3<T>::one(), v);
        }

        constexpr Transform3() noexcept :
            rotation(Quat<T>::identity()),
            scale(Vec3<T>::one()),
            translation(Vec3<T>::zero())
        {

        }
        constexpr Transform3(const Quat<T>& q, const Vec3<T>& s, const Vec3<T>& v) :
            rotation(q),
            scale(s),
            translation(v)
        {
        }
        constexpr Transform3(const Transform3<T>&) = default;
        constexpr Transform3(Transform3<T>&&) noexcept = default;
        constexpr Transform3& operator=(const Transform3<T>&) = default;
        constexpr Transform3& operator=(Transform3<T>&&) noexcept = default;

        Transform3<T> inverse() const
        {
            const Quat<T> invRotation = rotation.inverse();
            const Vec3<T> invScale = T(1) / scale;
            const Vec3<T> invTranslation = invRotation.apply(invScale * -translation);

            return Transform3(invRotation, invScale, invTranslation);
        }

        // rhs before this
        Transform3<T>& operator*=(const Transform3<T>& rhs)
        {
            //	When Q = quaternion, S = single scalar scale, and T = translation
            //	QST(A) = Q(A), S(A), T(A), and QST(B) = Q(B), S(B), T(B)

            //	QST (AB) 

            // QST(A) = Q(A)*S(A)*P*-Q(A) + T(A)
            // QST(AB) = Q(A)*S(A)*QST(B)*-Q(A) + T(A)
            // QST(AB) = Q(A)*S(A)*[Q(B)*S(B)*P*-Q(B) + T(B)]*-Q(A) + T(A)
            // QST(AB) = Q(A)*S(A)*Q(B)*S(B)*P*-Q(A)*-Q(A) + Q(A)*S(A)*T(B)*-Q(A) + T(A)
            // QST(AB) = [Q(A)*Q(B)]*[S(A)*S(B)]*P*-[Q(A)*Q(B)] + Q(A)*S(A)*T(B)*-Q(A) + T(A)

            //	Q(AB) = Q(A)*Q(B)
            //	S(AB) = S(B)*S(A)
            //	T(AB) = Q(A)*S(A)*T(B)*-Q(A) + T(A)

            // does not handle negative or zero scale

            const Quat<T> r = rotation * rhs.rotation;
            const Vec3<T> s = rhs.scale * scale;
            const Vec3<T> t = rotation.apply(scale * rhs.translation) + translation;

            rotation = r;
            scale = s;
            translation = t;

            return *this;
        }


        Vec3<T> apply(const Vec3<T>& v) const
        {
            return rotation.apply(v * scale) + translation;
        }

    private:

    };

    using Transform3D = Transform3<double>;
    using Transform3F = Transform3<float>;

    // rhs before lhs
    template <typename T>
    Transform3<T> operator*(const Transform3<T>& lhs, const Transform3<T>& rhs)
    {
        Transform3<T> result(lhs);
        return result *= rhs;
    }

    template <typename T>
    Vec3<T> operator*(const Vec3<T>& lhs, const Transform3<T>& rhs)
    {
        return rhs.apply(lhs);
    }
}

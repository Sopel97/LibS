#pragma once

#include "Vec3.h"

#include <cmath>

namespace ls
{
    template <typename T>
    struct Plane3
    {
    public:
        // ax + by + cz + d = 0
        T a;
        T b;
        T c;
        T d;

        constexpr Plane3(const T& a, const T& b, const T& c, const T& d) :
            a(a),
            b(b),
            c(c),
            d(d)
        {

        }

        constexpr void normalize()
        {
            using std::sqrt;

            const T invLen = T(1)/sqrt(a*a + b*b + c*c);
            a *= invLen;
            b *= invLen;
            c *= invLen;
            d *= invLen;
        }

        constexpr Plane3<T> normalized() const
        {
            Plane3<T> copy(*this);
            copy.normalize();
            return copy;
        }
    };

    using Plane3F = Plane3<float>;
    using Plane3D = Plane3<double>;
}

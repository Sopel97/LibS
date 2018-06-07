#pragma once

#include "Fwd.h"

#include "NoiseUtil.h"
#include "LibS/Common.h"
#include "LibS/Shapes/Vec2.h"
#include "LibS/Shapes/Vec3.h"
#include "LibS/Shapes/Vec4.h"

#include <numeric>
#include <algorithm>
#include <cmath>
#include <limits>

// Uses slightly modified implementation by Stefan Gustavson (stegu@itn.liu.se) (2003-2005)
// Original code can be found on github.com/kev009/craftd/tree/master/plugins/survival/mapgen/noise

namespace ls
{
    template <typename T, typename HashT>
    struct PerlinNoise : protected HashT
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");

    public:
        using ValueType = T;
        using HashType = HashT;

        PerlinNoise() noexcept :
            HashType{}
        {
        }
        template <typename HashFwdT>
        PerlinNoise(HashFwdT&& hash) :
            HashType(std::forward<HashFwdT>(hash))
        {
        }

        T raw(T x, int px)
        {
            int ix0 = floorToInt(x); // Integer part of x
            T fx0 = x - ix0;       // Fractional part of x
            T fx1 = fx0 - T(1);

            uint32_t ix0p0 = periodic(ix0, px);
            uint32_t ix0p1 = periodic(ix0 + 1, px);

            T s = detail::fade(fx0);

            T n0 = grad1(HashType::operator()(ix0p0), fx0);
            T n1 = grad1(HashType::operator()(ix0p1), fx1);

            return T(0.188) * (detail::lerp(n0, n1, s));
        }

        T raw(const Vec2<T>& coords, const Vec2<int>& period)
        {
            T x = coords.x;
            T y = coords.y;
            int px = period.x;
            int py = period.y;

            int ix0 = floorToInt(x); // Integer part of x
            int iy0 = floorToInt(y); // Integer part of y
            T fx0 = x - ix0;        // Fractional part of x
            T fy0 = y - iy0;        // Fractional part of y
            T fx1 = fx0 - T(1);
            T fy1 = fy0 - T(1);

            uint32_t ix0p0 = periodic(ix0, px);
            uint32_t iy0p0 = periodic(iy0, py);
            uint32_t ix0p1 = periodic(ix0 + 1, px);
            uint32_t iy0p1 = periodic(iy0 + 1, py);

            T t = detail::fade(fy0);
            T s = detail::fade(fx0);

            T nx0 = grad2(HashType::operator()(ix0p0 + HashType::operator()(iy0p0)), fx0, fy0);
            T nx1 = grad2(HashType::operator()(ix0p0 + HashType::operator()(iy0p1)), fx0, fy1);
            T n0 = detail::lerp(nx0, nx1, t);

            nx0 = grad2(HashType::operator()(ix0p1 + HashType::operator()(iy0p0)), fx1, fy0);
            nx1 = grad2(HashType::operator()(ix0p1 + HashType::operator()(iy0p1)), fx1, fy1);
            T n1 = detail::lerp(nx0, nx1, t);


            return T(0.507) * (detail::lerp(n0, n1, s));
        }

        T raw(const Vec3<T>& coords, const Vec3<int>& period)
        {
            T x = coords.x;
            T y = coords.y;
            T z = coords.z;
            int px = period.x;
            int py = period.y;
            int pz = period.z;

            int ix0 = floorToInt(x); // Integer part of x
            int iy0 = floorToInt(y); // Integer part of y
            int iz0 = floorToInt(z); // Integer part of z
            T fx0 = x - ix0;        // Fractional part of x
            T fy0 = y - iy0;        // Fractional part of y
            T fz0 = z - iz0;        // Fractional part of z
            T fx1 = fx0 - T(1);
            T fy1 = fy0 - T(1);
            T fz1 = fz0 - T(1);

            uint32_t ix0p0 = periodic(ix0, px);
            uint32_t iy0p0 = periodic(iy0, py);
            uint32_t iz0p0 = periodic(iz0, pz);
            uint32_t ix0p1 = periodic(ix0 + 1, px);
            uint32_t iy0p1 = periodic(iy0 + 1, py);
            uint32_t iz0p1 = periodic(iz0 + 1, pz);

            T r = detail::fade(fz0);
            T t = detail::fade(fy0);
            T s = detail::fade(fx0);

            T nxy0 = grad3(HashType::operator()(ix0p0 + HashType::operator()(iy0p0 + HashType::operator()(iz0p0))), fx0, fy0, fz0);
            T nxy1 = grad3(HashType::operator()(ix0p0 + HashType::operator()(iy0p0 + HashType::operator()(iz0p1))), fx0, fy0, fz1);
            T nx0 = detail::lerp(nxy0, nxy1, r);

            nxy0 = grad3(HashType::operator()(ix0p0 + HashType::operator()(iy0p1 + HashType::operator()(iz0p0))), fx0, fy1, fz0);
            nxy1 = grad3(HashType::operator()(ix0p0 + HashType::operator()(iy0p1 + HashType::operator()(iz0p1))), fx0, fy1, fz1);
            T nx1 = detail::lerp(nxy0, nxy1, r);

            T n0 = detail::lerp(nx0, nx1, t);

            nxy0 = grad3(HashType::operator()(ix0p1 + HashType::operator()(iy0p0 + HashType::operator()(iz0p0))), fx1, fy0, fz0);
            nxy1 = grad3(HashType::operator()(ix0p1 + HashType::operator()(iy0p0 + HashType::operator()(iz0p1))), fx1, fy0, fz1);
            nx0 = detail::lerp(nxy0, nxy1, r);

            nxy0 = grad3(HashType::operator()(ix0p1 + HashType::operator()(iy0p1 + HashType::operator()(iz0p0))), fx1, fy1, fz0);
            nxy1 = grad3(HashType::operator()(ix0p1 + HashType::operator()(iy0p1 + HashType::operator()(iz0p1))), fx1, fy1, fz1);
            nx1 = detail::lerp(nxy0, nxy1, r);

            T n1 = detail::lerp(nx0, nx1, t);

            return T(0.936) * (detail::lerp(n0, n1, s));
        }

        T raw(const Vec4<T>& coords, const Vec4<int>& period)
        {
            T x = coords.x;
            T y = coords.y;
            T z = coords.z;
            T w = coords.w;
            int px = period.x;
            int py = period.y;
            int pz = period.z;
            int pw = period.w;

            int ix0 = floorToInt(x); // Integer part of x
            int iy0 = floorToInt(y); // Integer part of y
            int iz0 = floorToInt(z); // Integer part of y
            int iw0 = floorToInt(w); // Integer part of w
            T fx0 = x - ix0;        // Fractional part of x
            T fy0 = y - iy0;        // Fractional part of y
            T fz0 = z - iz0;        // Fractional part of z
            T fw0 = w - iw0;        // Fractional part of w
            T fx1 = fx0 - T(1);
            T fy1 = fy0 - T(1);
            T fz1 = fz0 - T(1);
            T fw1 = fw0 - T(1);

            uint32_t ix0p0 = periodic(ix0, px);
            uint32_t iy0p0 = periodic(iy0, py);
            uint32_t iz0p0 = periodic(iz0, pz);
            uint32_t iw0p0 = periodic(iw0, pw);
            uint32_t ix0p1 = periodic(ix0 + 1, px);
            uint32_t iy0p1 = periodic(iy0 + 1, py);
            uint32_t iz0p1 = periodic(iz0 + 1, pz);
            uint32_t iw0p1 = periodic(iw0 + 1, pw);

            T q = detail::fade(fw0);
            T r = detail::fade(fz0);
            T t = detail::fade(fy0);
            T s = detail::fade(fx0);

            T nxyz0 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p0 + HashType::operator()(iz0p0 + HashType::operator()(iw0p0)))), fx0, fy0, fz0, fw0);
            T nxyz1 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p0 + HashType::operator()(iz0p0 + HashType::operator()(iw0p1)))), fx0, fy0, fz0, fw1);
            T nxy0 = detail::lerp(nxyz0, nxyz1, q);

            nxyz0 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p0 + HashType::operator()(iz0p1 + HashType::operator()(iw0p0)))), fx0, fy0, fz1, fw0);
            nxyz1 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p0 + HashType::operator()(iz0p1 + HashType::operator()(iw0p1)))), fx0, fy0, fz1, fw1);
            T nxy1 = detail::lerp(nxyz0, nxyz1, q);

            T nx0 = detail::lerp(nxy0, nxy1, r);

            nxyz0 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p1 + HashType::operator()(iz0p0 + HashType::operator()(iw0p0)))), fx0, fy1, fz0, fw0);
            nxyz1 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p1 + HashType::operator()(iz0p0 + HashType::operator()(iw0p1)))), fx0, fy1, fz0, fw1);
            nxy0 = detail::lerp(nxyz0, nxyz1, q);

            nxyz0 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p1 + HashType::operator()(iz0p1 + HashType::operator()(iw0p0)))), fx0, fy1, fz1, fw0);
            nxyz1 = grad4(HashType::operator()(ix0p0 + HashType::operator()(iy0p1 + HashType::operator()(iz0p1 + HashType::operator()(iw0p1)))), fx0, fy1, fz1, fw1);
            nxy1 = detail::lerp(nxyz0, nxyz1, q);

            T nx1 = detail::lerp(nxy0, nxy1, r);

            T n0 = detail::lerp(nx0, nx1, t);

            nxyz0 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p0 + HashType::operator()(iz0p0 + HashType::operator()(iw0p0)))), fx1, fy0, fz0, fw0);
            nxyz1 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p0 + HashType::operator()(iz0p0 + HashType::operator()(iw0p1)))), fx1, fy0, fz0, fw1);
            nxy0 = detail::lerp(nxyz0, nxyz1, q);

            nxyz0 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p0 + HashType::operator()(iz0p1 + HashType::operator()(iw0p0)))), fx1, fy0, fz1, fw0);
            nxyz1 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p0 + HashType::operator()(iz0p1 + HashType::operator()(iw0p1)))), fx1, fy0, fz1, fw1);
            nxy1 = detail::lerp(nxyz0, nxyz1, q);

            nx0 = detail::lerp(nxy0, nxy1, r);

            nxyz0 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p1 + HashType::operator()(iz0p0 + HashType::operator()(iw0p0)))), fx1, fy1, fz0, fw0);
            nxyz1 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p1 + HashType::operator()(iz0p0 + HashType::operator()(iw0p1)))), fx1, fy1, fz0, fw1);
            nxy0 = detail::lerp(nxyz0, nxyz1, q);

            nxyz0 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p1 + HashType::operator()(iz0p1 + HashType::operator()(iw0p0)))), fx1, fy1, fz1, fw0);
            nxyz1 = grad4(HashType::operator()(ix0p1 + HashType::operator()(iy0p1 + HashType::operator()(iz0p1 + HashType::operator()(iw0p1)))), fx1, fy1, fz1, fw1);
            nxy1 = detail::lerp(nxyz0, nxyz1, q);

            nx1 = detail::lerp(nxy0, nxy1, r);

            T n1 = detail::lerp(nx0, nx1, t);

            return T(0.87) * (detail::lerp(n0, n1, s));
        }

    private:
        static T grad1(uint32_t hash, T x)
        {
            uint32_t h = hash & 0b1111u;
            T grad = T(1) + (h & 0b111u);   // Gradient value 1.0, 2.0, ..., 8.0
            if (h & 0b1000u) grad = -grad;   // Set a random sign for the gradient
            return (grad * x);              // Multiply the gradient with the distance
        }
        static T grad2(uint32_t hash, T x, T y)
        {
            uint32_t h = hash & 0b111u;      // Convert low 3 bits of hash code
            T u = h < 4 ? x : y;  // into 8 simple gradient directions,
            T v = h < 4 ? y : x;  // and compute the dot product with (x,y).
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? T(-2)*v : T(2)*v);
        }
        static T grad3(uint32_t hash, T x, T y, T z)
        {
            uint32_t h = hash & 0b1111u;     // Convert low 4 bits of hash code into 12 simple
            T u = h < 8 ? x : y; // gradient directions, and compute dot product.
            T v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? -v : v);
        }
        static T grad4(uint32_t hash, T x, T y, T z, T t)
        {
            uint32_t h = hash & 0b11111u;      // Convert low 5 bits of hash code into 32 simple
            T u = h < 24 ? x : y; // gradient directions, and compute dot product.
            T v = h < 16 ? y : z;
            T w = h < 8 ? z : t;
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? -v : v) + ((h & 0b100u) ? -w : w);
        }
        static constexpr uint8_t m_simplex[64][4] = {
            { 0, 1, 2, 3 },{ 0, 1, 3, 2 },{ 0, 0, 0, 0 },{ 0, 2, 3, 1 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 1, 2, 3, 0 },
            { 0, 2, 1, 3 },{ 0, 0, 0, 0 },{ 0, 3, 1, 2 },{ 0, 3, 2, 1 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 1, 3, 2, 0 },
            { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },
            { 1, 2, 0, 3 },{ 0, 0, 0, 0 },{ 1, 3, 0, 2 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 2, 3, 0, 1 },{ 2, 3, 1, 0 },
            { 1, 0, 2, 3 },{ 1, 0, 3, 2 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 2, 0, 3, 1 },{ 0, 0, 0, 0 },{ 2, 1, 3, 0 },
            { 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },
            { 2, 0, 1, 3 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 3, 0, 1, 2 },{ 3, 0, 2, 1 },{ 0, 0, 0, 0 },{ 3, 1, 2, 0 },
            { 2, 1, 0, 3 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 0, 0, 0, 0 },{ 3, 1, 0, 2 },{ 0, 0, 0, 0 },{ 3, 2, 0, 1 },{ 3, 2, 1, 0 }
        };
    };

    using PerlinNoiseD = PerlinNoise<double>;
    using PerlinNoiseF = PerlinNoise<float>;
}

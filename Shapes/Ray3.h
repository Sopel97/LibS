#pragma once

#include "Vec3.h"

#include <type_traits>
#include <tuple>

namespace ls
{
    template <typename T>
    struct Ray3
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec3<T>;

        constexpr Ray3() noexcept = default;

        constexpr Ray3(const Ray3<T>&) = default;
        constexpr Ray3(Ray3<T>&&) noexcept = default;

        Ray3<T>& operator=(const Ray3<T>&) = default;
        Ray3<T>& operator=(Ray3<T> &&) noexcept = default;

        constexpr Ray3(const Vec3<T>& origin, const Vec3<T>& direction) noexcept(std::is_nothrow_copy_constructible<Vec3<T>>::value) :
            m_origin(origin),
            m_direction(direction.normalized())
        {

        }

        constexpr Ray3<T> translated(const Vec3<T>& displacement) const
        {
            return Ray3<T>(m_origin + displacement, m_direction);
        }
        Ray3<T>& translate(const Vec3<T>& displacement)
        {
            m_origin += displacement;
            return *this;
        }

        template <typename T2>
        constexpr explicit operator Ray3<T2>() const
        {
            return Ray3<T2>(static_cast<Vec3<T2>>(m_origin), static_cast<Vec3<T2>>(m_direction));
        }

        constexpr const Vec3<T>& origin() const
        {
            return m_origin;
        }

        constexpr Vec3<T>& origin()
        {
            return m_origin;
        }

        constexpr const Vec3<T>& direction() const
        {
            return m_direction;
        }

        void setDirection(const Vec3<T>& newDirection)
        {
            m_direction = newDirection.normalized();
        }

    private:
        Vec3<T> m_origin;
        Vec3<T> m_direction;
    };

    using Ray3D = Ray3<double>;
    using Ray3F = Ray3<float>;

    template <typename T>
    constexpr bool operator== (const Ray3<T>& lhs, const Ray3<T>& rhs)
    {
        return std::tie(lhs.origin(), lhs.direction()) == std::tie(rhs.origin(), rhs.direction());
    }
    template <typename T>
    constexpr bool operator!= (const Ray3<T>& lhs, const Ray3<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

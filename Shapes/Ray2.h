#pragma once

#include "Vec2.h"

#include <type_traits>

namespace ls
{
    template <typename T>
    struct Ray2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        constexpr Ray2() noexcept = default;

        constexpr Ray2(const Ray2<T>&) = default;
        constexpr Ray2(Ray2<T>&&) noexcept = default;

        Ray2<T>& operator=(const Ray2<T>&) = default;
        Ray2<T>& operator=(Ray2<T> &&) noexcept = default;

        constexpr Ray2(const Vec2<T>& origin, const Vec2<T>& direction) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
            m_origin(origin),
            m_direction(direction.normalized())
        {

        }

        constexpr Ray2<T> translated(const Vec2<T>& displacement) const
        {
            return Ray2<T>(m_origin + displacement, m_direction);
        }

        Ray2<T>& translate(const Vec2<T>& displacement)
        {
            m_origin += displacement;
            return *this;
        }

        template <typename T2>
        constexpr explicit operator Ray2<T2>() const
        {
            return Ray2<T2>(static_cast<Vec2<T2>>(m_origin), static_cast<Vec2<T2>>(m_direction));
        }

        constexpr const Vec2<T>& origin() const
        {
            return m_origin;
        }

        constexpr Vec2<T>& origin()
        {
            return m_origin;
        }

        constexpr const Vec2<T>& direction() const
        {
            return m_direction;
        }

        void setDirection(const Vec2<T>& newDirection)
        {
            m_direction = newDirection.normalized();
        }

    private:
        Vec2<T> m_origin;
        Vec2<T> m_direction;
    };

    using Ray2D = Ray2<double>;
    using Ray2F = Ray2<float>;

    template <typename T>
    constexpr bool operator== (const Ray2<T>& lhs, const Ray2<T>& rhs)
    {
        return std::tie(lhs.origin(), lhs.direction()) == std::tie(rhs.origin(), rhs.direction());
    }
    template <typename T>
    constexpr bool operator!= (const Ray2<T>& lhs, const Ray2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

#pragma once

#include <type_traits>
#include <utility>

#include "Angle2.h"
#include "../Macros.h"

namespace ls
{
    /**
     * typename representing a 2 dimensional vector
     */
    template <typename T>
    struct Vec2
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;
        using VectorType = Vec2<T>;

        T x, y;

        /**
         * Default constructor
         *
         * Does not initialize the components
         */
        constexpr Vec2() noexcept = default;

        /**
        * Default copy constructor
        */
        constexpr Vec2(const Vec2<T>&) = default;

        /**
        * Default move constructor
        */
        constexpr Vec2(Vec2<T>&&) noexcept = default;

        /**
        * Default copy assign
        */
        Vec2<T>& operator=(const Vec2<T>&) = default;

        /**
        * Default move assign
        */
        Vec2<T>& operator=(Vec2<T> &&) noexcept = default;

        /**
         * Constructor
         *
         * @param xy Value to be assigned to both components
         */
        constexpr explicit Vec2(const T& xy) noexcept(std::is_nothrow_copy_constructible<T>::value) :
            x(xy),
            y(xy)
        {
        }

        /**
        * Constructor
        *
        * @param x Value to be assigned to the x component
        * @param y Value to be assigned to the y component
        */
        constexpr Vec2(const T& x, const T& y) noexcept(std::is_nothrow_copy_constructible<T>::value) :
            x(x),
            y(y)
        {
        }

        /**
        * Named constructor that returns a unit vector with given angle
        *
        * @param angle Angle of the result vector
        * @result Unit vector with given angle
        */
        static constexpr Vec2<T> direction(const Angle2<T>& angle) noexcept(std::is_nothrow_constructible<Vec2<T>, const T&, const T&>::value)
        {
            return Vec2<T>(angle.cos(), angle.sin());
        }

        /**
        * Named constructor that returns a zero vector
        *
        * @return A zero vector
        */
        static constexpr const Vec2<T>& zero() noexcept
        {
            static constexpr Vec2<T> vec(T(0), T(0));
            return vec;
        }

        /**
        * Named constructor that returns a (1, 1) vector
        *
        * @return A (1, 1) vector
        */
        static constexpr const Vec2<T>& one() noexcept
        {
            static constexpr Vec2<T> vec(T(1), T(1));
            return vec;
        }

        /**
        * Named constructor returning unit vector on OX axis
        *
        * @return Vector (1, 0)
        */
        static constexpr const Vec2<T>& unitX() noexcept
        {
            static constexpr Vec2<T> vec(T(1), T(0));
            return vec;
        }

        /**
        * Named constructor returning unit vector on OY axis
        *
        * @return Vector (0, 1)
        */
        static constexpr const Vec2<T>& unitY() noexcept
        {
            static constexpr Vec2<T> vec(T(0), T(1));
            return vec;
        }

        /**
        * Adds a vector to this
        *
        * @param rhs Vector to be added to this
        * @return reference to this after operation
        */
        Vec2<T>& operator+=(const Vec2<T>& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        /**
        * Subtracts a vector from this
        *
        * @param rhs Vector to be subtracted from this
        * @return reference to this after operation
        */
        Vec2<T>& operator-=(const Vec2<T>& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        /**
        * Multiplies this vector in place by a scalar
        *
        * @param rhs Scalar to multiply by
        * @return reference to this after operation
        */
        Vec2<T>& operator*=(const T& rhs)
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        /**
        * Multiplies this vector in place by a another one
        * This is component-wise multiplication
        *
        * @param rhs Vector to multiply by
        * @return reference to this after operation
        */
        Vec2<T>& operator*=(const Vec2<T>& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            return *this;
        }

        /**
        * Divides this vector in place by a scalar
        *
        * @param rhs Scalar to divide by
        * @return reference to this after operation
        */
        Vec2<T>& operator/=(const T& rhs)
        {
            x /= rhs;
            y /= rhs;
            return *this;
        }

        /**
        * Divides this vector in place by a another one
        * This is component-wise division
        *
        * @param rhs Vector to divide by
        * @return reference to this after operation
        */
        Vec2<T>& operator/=(const Vec2<T>& rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
            return *this;
        }

        /**
        * Accesses component by index
        *
        * @param i Zero-based index of the component
        * @return Const refence to the component
        */
        const T& operator[](int i) const
        {
            return (&x)[i];
        }

        /**
        * Accesses component by index
        *
        * @param i Zero-based index of the component
        * @return Reference to the component
        */
        T& operator[](int i)
        {
            return (&x)[i];
        }

        /**
        * Converts to a Vec2 with different underlying type
        */
        template <typename T2>
        constexpr explicit operator Vec2<T2>() const
        {
            return Vec2<T2>(static_cast<T2>(x), static_cast<T2>(y));
        }

        /**
        * Returns the length of the vector
        *
        * @return The length of this vector
        */
        constexpr T length() const
        {
            using std::sqrt;

            return static_cast<T>(sqrt(lengthSquared()));
        }

        /**
        * Returns the squared length of the vector
        *
        * @return The squared length of this vector
        */
        constexpr T lengthSquared() const
        {
            return x * x + y * y;
        }

        /**
        * Normalizes this vector in place (divides itself by the length)
        */
        void normalize()
        {
            const T invertedMagnitude = T(1) / length();
            x *= invertedMagnitude;
            y *= invertedMagnitude;
        }

        /**
        * Returns a normalized copy of this
        *
        * @return A normalized copy of this
        */
        Vec2<T> normalized() const
        {
            Vec2<T> result(*this);
            result.normalize();
            return result;
        }

        /**
        * Returns the anticlockwise normal (with unit length) of this vector
        *
        * @return The anticlockwise normal
        */
        Vec2<T> normalAnticlockwise() const
        {
            return Vec2<T>(-y, x).normalized();
        }

        /**
        * Returns the clockwise normal (with unit length) of this vector
        *
        * @return The clockwise normal
        */
        Vec2<T> normalClockwise() const
        {
            return Vec2<T>(y, -x).normalized();
        }

        /**
        * Returns either a clockwise or an anticlockwise normal, it is not specified which one
        *
        * @return A normal to this
        */
        Vec2<T> normal() const
        {
            return normalAnticlockwise();
        }

        /**
        * Performs a dot product on this and a given vector
        *
        * @param rhs Vector to dot with
        * @return The dot product of the two vectors
        */
        constexpr T dot(const Vec2<T>& rhs) const
        {
            return x * rhs.x + y * rhs.y;
        }

        /**
        * Performs a cross product on this and a given vector
        * Determinant of a 2x2 matrix made by these vectors
        *
        * @param rhs Vector to cross with
        * @return The cross product of the two vectors
        */
        constexpr T cross(const Vec2<T>& rhs) const
        {
            return x * rhs.y - y * rhs.x;
        }

        /**
        * Projects this onto an axis
        * Does not modify this
        *
        * @param axis Axis to project onto
        * @return The projected vector
        */
        Vec2<T> projected(const Vec2<T>& axis) const
        {
            const Vec2<T> n = axis.normalized();
            return n * dot(n);
        }

        /**
        * Mirrors this in place across given vector
        *
        * @param mirror Mirroring line vector
        */
        void mirror(const Vec2<T>& mirror)
        {
            *this = -*this + T(2)*mirror*(dot(mirror));
        }

        /**
        * Mirrors this across given vector
        * Does not modify this
        *
        * @param mirror Mirroring line vector
        * @return Mirrored this
        */
        Vec2<T> mirrored(const Vec2<T>& mirror) const
        {
            Vec2<T> result(*this);
            result.mirror(mirror);
            return result;
        }

        /**
        * Rotates (anticlockwise) the vector by a given angle
        *
        * @param angle Angle to rotate by
        */
        void rotate(const Angle2<T>& angle)
        {
            rotate(direction(angle));
        }

        /**
        * Rotates (anticlockwise) the vector by a given angle
        * Does not modify this
        *
        * @param angle Angle to rotate by
        * @return Copy of this rotated by angle
        */
        Vec2<T> rotated(const Angle2<T>& angle) const
        {
            return rotated(direction(angle));
        }

        /**
        * Rotates this in palce by a given vector (as complex number would)
        *
        * @param vector Vector to rotate by (must be normalized)
        */
        void rotate(const Vec2<T>& vector)
        {
            const T xx = x;
            const T yy = y;

            x = xx * vector.x - yy * vector.y;
            y = xx * vector.y + yy * vector.x;
        }

        /**
        * Rotates the vector by a given vector (as complex number would)
        * Does not modify this
        *
        * @param vector Vector to rotate by (must be normalized)
        * @return Copy of this rotated by vector
        */
        Vec2<T> rotated(const Vec2<T>& vector) const
        {
            Vec2<T> result(*this);
            result.rotate(vector);
            return result;
        }

        /**
        * Computes the shorter angle between this and (1, 0)
        *
        * @return Angle of the vector
        */
        constexpr Angle2<T> angle() const
        {
            return Angle2<T>::atan2(y, x);
        }

        /**
        * Computes the shorter angle between two vectors
        *
        * @param other Vector to compute angle with
        * @return Angle between vectors
        */
        constexpr Angle2<T> angle(const Vec2<T>& other) const
        {
            return Angle2<T>::acos(dot(other) / (other.length() * length()));
        }

        /**
        * Computes the distance between to vectors
        *
        * @param other Vector to compute distance to
        * @return Distance between vectors
        */
        constexpr T distance(const Vec2<T>& other) const
        {
            return (other - *this).length();
        }

        /**
        * Computes the squared distance between to vectors
        *
        * @param other Vector to compute squared distance to
        * @return Squared distance between vectors
        */
        constexpr T distanceSquared(const Vec2<T>& other) const
        {
            return (other - *this).lengthSquared();
        }

    };

    using Vec2D = Vec2<double>;
    using Vec2F = Vec2<float>;
    using Vec2I = Vec2<int>;

    /**
     * Compares vectors for equality
     *
     * @param lhs First vector
     * @param rhs Second vector
     * @return Whether vector are exactly equal
     */
    template <typename T>
    constexpr bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
    }

    /**
    * Compares vectors for inequality
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether vector are not exactly equal
    */
    template <typename T>
    constexpr bool operator!=(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return !(lhs == rhs);
    }

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less than rhs
    */
    template <typename T>
    constexpr bool operator<(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater than rhs
    */
    template <typename T>
    constexpr bool operator>(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return rhs < lhs;
    }

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less or equal than rhs
    */
    template <typename T>
    constexpr bool operator<=(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return !(rhs < lhs);
    }

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater or equal than rhs
    */
    template <typename T>
    constexpr bool operator>=(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return !(lhs < rhs);
    }

    /**
    * Returns negation of a vector
    *
    * @param vector Vector to negate
    * @return Negated copy of a vector
    */
    template <typename T>
    constexpr Vec2<T> operator-(const Vec2<T>& vector)
    {
        return Vec2<T>(-vector.x, -vector.y);
    }

    /**
    * Adds two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Sum of the two vectors
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() + std::declval<T2>())>
    constexpr Vec2<R> operator+(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x + rhs.x,
            lhs.y + rhs.y
            );
    }

    /**
    * Subtracts two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs - rhs
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() - std::declval<T2>())>
    constexpr Vec2<R> operator-(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x - rhs.x,
            lhs.y - rhs.y
            );
    }

    /**
    * Multiplies vector by scalar
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <typename T1, typename T2, typename R = std::enable_if_t<std::is_arithmetic_v<T1>, decltype(std::declval<T1>() * std::declval<T2>())>>
    constexpr Vec2<R> operator*(const Vec2<T1>& lhs, const T2& rhs)
    {
        return Vec2<R>(
            lhs.x * rhs,
            lhs.y * rhs
            );
    }

    /**
    * Divides vector by scalar
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs/rhs
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec2<R> operator/(const Vec2<T1>& lhs, const T2& rhs)
    {
        return Vec2<R>(
            lhs.x / rhs,
            lhs.y / rhs
            );
    }

    /**
    * Multiplies vector by a scalar (takes them in inverted order)
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec2<R> operator*(const T1& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs * rhs.x,
            lhs * rhs.y
            );
    }

    /**
    * Divides scalar (converted to a vector) by a vector
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Vec2(lhs, lhs)/rhs
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec2<R> operator/(const T1& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs / rhs.x,
            lhs / rhs.y
            );
    }

    /**
    * Component-wise multiplies two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Component-wise lhs*rhs
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec2<R> operator*(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x * rhs.x,
            lhs.y * rhs.y
            );
    }

    /**
    * Component-wise divides two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Component-wise lhs/rhs
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec2<R> operator/(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x / rhs.x,
            lhs.y / rhs.y
            );
    }

    template <typename T>
    constexpr bool almostZero(const Vec2<T>& val, const T& tolerance)
    {
        using std::abs;

        return abs(val.x) <= tolerance && abs(val.y) <= tolerance;
    }

    template <typename T>
    constexpr bool almostEqual(const Vec2<T>& lhs, const Vec2<T>& rhs, const T& tolerance)
    {
        using std::abs;

        return abs(lhs.x - rhs.x) <= tolerance && abs(lhs.y - rhs.y) <= tolerance;
    }

    template <typename FloatType, typename IntType = int>
    Vec2<IntType> floorToInt(const Vec2<FloatType>& value)
    {
        return { floorToInt(value.x), floorToInt(value.y) };
    }
}

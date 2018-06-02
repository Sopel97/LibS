#pragma once

#include <type_traits>

namespace ls
{
    namespace detail
    {
        template <typename T>
        struct vec4_align
        {
            static constexpr size_t value = alignof(T);
        };

        template <>
        struct vec4_align<float>
        {
            static constexpr size_t value = 16;
        };
    }

    /**
    * Class representing 4 dimensional vector
    * If the underlying type is float then it has specified align of 16 bytes
    */
    template <typename T>
    struct alignas(detail::vec4_align<T>::value) Vec4
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;
        using VectorType = Vec4<T>;

        T x, y, z, w;

        /**
        * Default constructor
        *
        * Does not initialize the components
        */
        constexpr Vec4() noexcept = default;

        /**
        * Constructor
        *
        * @param xyzw Value to be assigned to all components
        */
        constexpr explicit Vec4(const T& xyzw) noexcept(std::is_nothrow_copy_constructible<T>::value) :
            x(xyzw),
            y(xyzw),
            z(xyzw),
            w(xyzw)
        {
        }

        /**
        * Constructor
        *
        * @param x Value to be assigned to the x component
        * @param y Value to be assigned to the y component
        * @param z Value to be assigned to the z component
        * @param w Value to be assigned to the w component
        */
        constexpr Vec4(const T& x, const T& y, const T& z, const T& w) noexcept(std::is_nothrow_copy_constructible<T>::value) :
            x(x),
            y(y),
            z(z),
            w(w)
        {
        }

        /**
        * Named constructor that returns a zero vector
        *
        * @return A zero vector
        */
        static constexpr const Vec4<T>& zero() noexcept
        {
            static constexpr Vec4<T> vec(T(0), T(0), T(0), T(0));
            return vec;
        }

        /**
        * Named constructor that returns a (1, 1, 1, 1) vector
        *
        * @return A (1, 1, 1, 1) vector
        */
        static constexpr const Vec4<T>& one() noexcept
        {
            static constexpr Vec4<T> vec(T(1), T(1), T(1), T(1));
            return vec;
        }

        /**
        * Named constructor returning unit vector on OX axis
        *
        * @return Vector (1, 0, 0, 0)
        */
        static constexpr const Vec4<T>& unitX() noexcept
        {
            static constexpr Vec4<T> vec(T(1), T(0), T(0), T(0));
            return vec;
        }

        /**
        * Named constructor returning unit vector on OY axis
        *
        * @return Vector (0, 1, 0, 0)
        */
        static constexpr const Vec4<T>& unitY() noexcept
        {
            static constexpr Vec4<T> vec(T(0), T(1), T(0), T(0));
            return vec;
        }

        /**
        * Named constructor returning unit vector on OZ axis
        *
        * @return Vector (0, 0, 1, 0)
        */
        static constexpr const Vec4<T>& unitZ() noexcept
        {
            static constexpr Vec4<T> vec(T(0), T(0), T(1), T(0));
            return vec;
        }

        /**
        * Named constructor returning unit vector on OW axis
        *
        * @return Vector (0, 0, 0, 1)
        */
        static constexpr const Vec4<T>& unitW() noexcept
        {
            static constexpr Vec4<T> vec(T(0), T(0), T(0), T(1));
            return vec;
        }

        /**
        * Default copy constructor
        */
        constexpr Vec4(const Vec4<T>&) = default;

        /**
        * Default move constructor
        */
        constexpr Vec4(Vec4<T>&&) noexcept = default;

        /**
        * Default copy assign
        */
        Vec4<T>& operator=(const Vec4<T>&) = default;

        /**
        * Default move assign
        */
        Vec4<T>& operator=(Vec4<T>&&) noexcept = default;

        /**
        * Adds a vector to this
        *
        * @param rhs Vector to be added to this
        * @return reference to this after operation
        */
        Vec4<T>& operator+=(const Vec4<T>& rhs)
        {
            x += rhs.x;
            y += rhs.y;
            z += rhs.z;
            w += rhs.w;
            return *this;
        }

        /**
        * Subtracts a vector from this
        *
        * @param rhs Vector to be subtracted from this
        * @return reference to this after operation
        */
        Vec4<T>& operator-=(const Vec4<T>& rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            z -= rhs.z;
            w -= rhs.w;
            return *this;
        }

        /**
        * Multiplies this vector in place by a scalar
        *
        * @param rhs Scalar to multiply by
        * @return reference to this after operation
        */
        Vec4<T>& operator*=(const T& rhs)
        {
            x *= rhs;
            y *= rhs;
            z *= rhs;
            w *= rhs;
            return *this;
        }

        /**
        * Multiplies this vector in place by a another one
        * This is component-wise multiplication
        *
        * @param rhs Vector to multiply by
        * @return reference to this after operation
        */
        Vec4<T>& operator*=(const Vec4<T>& rhs)
        {
            x *= rhs.x;
            y *= rhs.y;
            z *= rhs.z;
            w *= rhs.w;
            return *this;
        }

        /**
        * Divides this vector in place by a scalar
        *
        * @param rhs Scalar to divide by
        * @return reference to this after operation
        */
        Vec4<T>& operator/=(const T& rhs)
        {
            x /= rhs;
            y /= rhs;
            z /= rhs;
            w /= rhs;
            return *this;
        }

        /**
        * Divides this vector in place by a another one
        * This is component-wise division
        *
        * @param rhs Vector to divide by
        * @return reference to this after operation
        */
        Vec4<T>& operator/=(const Vec4<T>& rhs)
        {
            x /= rhs.x;
            y /= rhs.y;
            z /= rhs.z;
            w /= rhs.w;
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
        * @return Const refence to the component
        */
        T& operator[](int i)
        {
            return (&x)[i];
        }

        /**
        * Converts to a Vec4 with different underlying type
        */
        template <typename T2>
        constexpr explicit operator Vec4<T2>() const
        {
            return Vec4<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
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
        * @return The quared length of this vector
        */
        constexpr T lengthSquared() const
        {
            return static_cast<T>((x*x) + (y*y) + (z*z) + (w*w));
        }

        /**
        * Computes the distance between to vectors
        *
        * @param other Vector to compute distance to
        * @return Distance between vectors
        */
        constexpr T distance(const Vec4<T>& other) const
        {
            return (other - *this).length();
        }

        /**
        * Computes the squared distance between to vectors
        *
        * @param other Vector to compute squared distance to
        * @return Squared distance between vectors
        */
        constexpr T distanceSquared(const Vec4<T>& other) const
        {
            return (other - *this).lengthSquared();
        }

        /**
        * Normalizes this vector in place (divides itself by the length)
        */
        void normalize()
        {
            const T invMag = T(1) / length();
            x *= invMag;
            y *= invMag;
            z *= invMag;
            w *= invMag;
        }

        /**
        * Returns a normalized copy of this
        *
        * @return A normalized copy of this
        */
        Vec4<T> normalized() const
        {
            Vec4<T> result(*this);
            result.normalize();
            return result;
        }

        /**
        * Performs a dot product on this and a given vector
        *
        * @param rhs Vector to dot with
        * @return The dot product of the two vectors
        */
        constexpr T dot(const Vec4<T>& rhs) const
        {
            return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
        }
    };

    using Vec4D = Vec4<double>;
    using Vec4F = Vec4<float>;
    using Vec4I = Vec4<int>;

    /**
    * Compares vectors for equality
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether vector are exactly equal
    */
    template <typename T>
    constexpr bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
    }

    /**
    * Compares vectors for inequality
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether vector are not exactly equal
    */
    template <typename T>
    constexpr bool operator!=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(lhs == rhs);
    }

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less than rhs
    */
    template <typename T>
    constexpr bool operator<(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) < std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
    }

    /**
    * Orders vectors (total order)
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater than rhs
    */
    template <typename T>
    constexpr bool operator>(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return rhs < lhs;
    }

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is less or equal than rhs
    */
    template <typename T>
    constexpr bool operator<=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(rhs < lhs);
    }

    /**
    * Orders vectors
    * First compares x, if it is equal then compares y, and so on
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Whether lhs is greater or equal than rhs
    */
    template <typename T>
    constexpr bool operator>=(const Vec4<T>& lhs, const Vec4<T>& rhs)
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
    constexpr Vec4<T> operator-(const Vec4<T>& vector)
    {
        return Vec4<T>(-vector.x, -vector.y, -vector.z, -vector.w);
    }

    /**
    * Adds two vectors
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return Sum of the two vectors
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() + std::declval<T2>())>
    constexpr Vec4<R> operator+(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z,
            lhs.w + rhs.w
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
    constexpr Vec4<R> operator-(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z,
            lhs.w - rhs.w
            );
    }

    /**
    * Multiplies vector by scalar
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <typename T1, typename T2, typename R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const T2& rhs)
    {
        return Vec4<R>(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs,
            lhs.w * rhs
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
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const T2& rhs)
    {
        return Vec4<R>(
            lhs.x / rhs,
            lhs.y / rhs,
            lhs.z / rhs,
            lhs.w / rhs
            );
    }

    /**
    * Multiplies vector by a scalar (takes them in inverted order)
    *
    * @param lhs First vector
    * @param rhs Second vector
    * @return lhs*rhs
    */
    template <typename T1, typename T2, typename R = std::enable_if_t<std::is_arithmetic_v<T1>, decltype(std::declval<T1>() * std::declval<T2>())>>
    constexpr Vec4<R> operator*(const T1& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z,
            lhs * rhs.w
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
    constexpr Vec4<R> operator/(const T1& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z,
            lhs / rhs.w
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
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z,
            lhs.w * rhs.w
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
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x / rhs.x,
            lhs.y / rhs.y,
            lhs.z / rhs.z,
            lhs.w / rhs.w
            );
    }

    template <typename T>
    constexpr bool almostZero(const Vec4<T>& val, const T& tolerance)
    {
        using std::abs;

        return abs(val.x) <= tolerance && abs(val.y) <= tolerance && abs(val.z) <= tolerance && abs(val.w) <= tolerance;
    }
    template <typename T>
    constexpr bool almostEqual(const Vec4<T>& lhs, const Vec4<T>& rhs, const T& tolerance)
    {
        using std::abs;

        return abs(lhs.x - rhs.x) <= tolerance && abs(lhs.y - rhs.y) <= tolerance && abs(lhs.z - rhs.z) <= tolerance && abs(lhs.w - rhs.w) <= tolerance;
    }
    template <typename FloatType, typename IntType = int>
    Vec4<IntType> floorToInt(const Vec4<FloatType>& value)
    {
        return { floorToInt(value.x), floorToInt(value.y), floorToInt(value.z), floorToInt(value.w) };
    }
}

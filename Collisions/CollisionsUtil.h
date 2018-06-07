#pragma once

#include "LibS/Collisions/Fwd.h"

#include "LibS/Shapes/Vec2.h"

namespace ls
{
    template <typename T>
    struct ContinuousCollision2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    private:
        Vec2<T> m_point;
        Vec2<T> m_normal;
        T m_timeToImpact;

    public:
        ContinuousCollision2(const Vec2<T>& point, const Vec2<T>& normal, const T& timeToImpact) :
            m_point(point),
            m_normal(normal),
            m_timeToImpact(timeToImpact)
        {

        }

        const Vec2<T>& normal() const
        {
            return m_normal;
        }
        const T& timeToImpact() const
        {
            return m_timeToImpact;
        }
    };

    using ContinuousCollision2D = ContinuousCollision2<double>;
    using ContinuousCollision2F = ContinuousCollision2<float>;

    template <typename ShapeT, typename VectorT>
    struct Accelerating
    {
    private:
        const ShapeT* m_shape;
        VectorT m_acceleration;
        VectorT m_velocity;
    public:
        Accelerating(const ShapeT& shape, const VectorT& acceleration, const VectorT& velocity) :
            m_shape(&shape),
            m_acceleration(acceleration),
            m_velocity(velocity)
        {

        }

        const ShapeT& shape() const
        {
            return *m_shape;
        }
        const VectorT& acceleration() const
        {
            return m_acceleration;
        }
        const VectorT& velocity() const
        {
            return m_velocity;
        }
    };

    template <typename ShapeT, typename VectorT>
    struct Moving
    {
    private:
        const ShapeT* m_shape;
        VectorT m_velocity;
    public:

        Moving(const ShapeT& shape, const VectorT& velocity) :
            m_shape(&shape),
            m_velocity(velocity)
        {

        }

        const ShapeT& shape() const
        {
            return *m_shape;
        }
        const VectorT& velocity() const
        {
            return m_velocity;
        }
    };

    template <typename ShapeT, typename VectorT = typename ShapeT::VectorType>
    Accelerating<ShapeT> as_accelerating(const ShapeT& shape, const VectorT& acceleration = VectorT::zero(), const VectorT& velocity = VectorT::zero())
    {
        return Accelerating<ShapeT>(shape, acceleration, velocity);
    }
    template <typename ShapeT, typename VectorT = typename ShapeT::VectorType>
    Moving<ShapeT> as_moving(const ShapeT& shape, const VectorT& velocity = VectorT::zero())
    {
        return Moving<ShapeT>(shape, velocity);
    }

    template <typename T>
    struct PointNormalPair2
    {
        Vec2<T> point;
        Vec2<T> normal;
    };
}

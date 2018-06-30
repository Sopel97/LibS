#pragma once

#include "LibS/Shapes/Vec2.h"

#include "Fwd.h"

namespace ls
{
    template <typename ShapeT>
    struct WithVelocity
    {
    public:
        using VectorType = typename ShapeT::VectorType;
        using ShapeType = ShapeT;

        WithVelocity(const ShapeT& shape, const VectorType& velocity) :
            m_shape(shape),
            m_velocity(velocity)
        {

        }

        const ShapeT& shape() const
        {
            return m_shape;
        }
        const VectorType& velocity() const
        {
            return m_velocity;
        }

    private:
        const ShapeT& m_shape;
        VectorType m_velocity;
    };

    template <typename ShapeT>
    WithVelocity<ShapeT> withVelocity(const ShapeT& shape, const typename ShapeT::VectorType& velocity)
    {
        return WithVelocity<ShapeT>(shape, velocity);
    }

    template <typename T>
    struct PointNormalPair2
    {
        Vec2<T> point;
        Vec2<T> normal;
    };
}

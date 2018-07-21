#pragma once

#include "LibS/Shapes.h"

namespace ls
{
    namespace detail
    {
        template <template <typename> typename ResultShapeTT>
        struct BoundingImpl;

        template <>
        struct BoundingImpl<Box2>
        {
            template <typename T>
            static Box2<T> compute(const Circle2<T>& circle)
            {
                const Vec2<T> halfDiagonal(circle.radius, circle.radius);
                return Box2<T>(circle.origin - halfDiagonal, circle.origin + halfDiagonal);
            }
        };
    }

    template <template <typename> typename ResultShapeTT, typename ShapeT>
    ResultShapeTT<typename ShapeT::ValueType> bounding(const ShapeT& s)
    {
        return detail::BoundingImpl<ResultShapeTT>::compute(s);
    }
}
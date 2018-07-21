#pragma once

#include "ShapeClosestPoints.h"

#include "LibS/Shapes.h"

namespace ls
{
    template <typename ShapeLhsT, typename ShapeRhsT, typename T = std::common_type_t<ShapeLhsT::ValueType, ShapeRhsT::ValueType>>
    T distance(const ShapeLhsT& lhs, const ShapeRhsT& rhs)
    {
        return closestPoints(lhs, rhs).distance();
    }

    template <typename ShapeLhsT, typename ShapeRhsT, typename T = std::common_type_t<ShapeLhsT::ValueType, ShapeRhsT::ValueType>>
    T distanceSquared(const ShapeLhsT& lhs, const ShapeRhsT& rhs)
    {
        return closestPoints(lhs, rhs).distanceSquared();
    }

    template <typename T>
    T distance(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return lhs.distance(rhs);
    }

    template <typename T>
    T distanceSquared(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return lhs.distanceSquared(rhs);
    }

    template <typename T>
    T distance(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return lhs.distance(rhs);
    }

    template <typename T>
    T distanceSquared(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return lhs.distanceSquared(rhs);
    }

    template <typename T>
    T distance(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        const T originDistance = distance(lhs.origin, rhs.origin);
        const T radiusSum = lhs.radius + rhs.radius;
        if (originDistance >= radiusSum)
        {
            return originDistance - radiusSum;
        }
        else
        {
            return static_cast<T>(0);
        }
    }

    template <typename T>
    T distanceSquared(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        const T d = distance(lhs, rhs);
        return d * d;
    }

    template <typename T>
    T distance(const Sphere3<T>& lhs, const Sphere3<T>& rhs)
    {
        const T originDistance = distance(lhs.origin, rhs.origin);
        const T radiusSum = lhs.radius + rhs.radius;
        if (originDistance >= radiusSum)
        {
            return originDistance - radiusSum;
        }
        else
        {
            return static_cast<T>(0);
        }
    }

    template <typename T>
    T distanceSquared(const Sphere3<T>& lhs, const Sphere3<T>& rhs)
    {
        const T d = distance(lhs, rhs);
        return d * d;
    }
}
#pragma once

#include "ShapeIntersections2.h"

#include "LibS/Shapes.h"

#include <utility>

namespace ls
{
    template <typename T>
    struct ClosestPointsPair
    {
        Vec2<T> lhs;
        Vec2<T> rhs;

        T distance() const
        {
            return lhs.distance(rhs);
        }

        T distanceSquared() const
        {
            return lhs.distanceSquared(rhs);
        }

        ClosestPointsPair<T> swapped()
        {
            ClosestPointsPair<T> ret(*this);
            ret.swapPoints();
            return ret;
        }

        void swapPoints()
        {
            using std::swap;

            swap(ret.lhs, ret.rhs);
        }
    };

    template <typename T>
    ClosestPointsPair<T> closestPoints(const ls::Vec2<T>& lhs, const ls::Vec2<T>& rhs)
    {
        return { lhs, rhs };
    }

    template <typename T>
    ClosestPointsPair<T> closestPoints(const ls::Vec2<T>& lhs, const ls::Circle2<T>& rhs)
    {
        if (intersect(lhs, rhs))
        {
            return { lhs, lhs };
        }

        return { lhs, rhs.origin + (lhs - rhs.origin).normalized() * rhs.radius };
    }

    template <typename T>
    ClosestPointsPair<T> closestPoints(const ls::Circle2<T>& lhs, const ls::Vec2<T>& rhs)
    {
        return closestPoints(rhs, lhs).swapped();
    }

    template <typename T>
    ClosestPointsPair<T> closestPoints(const ls::Circle2<T>& lhs, const ls::Circle2<T>& rhs)
    {
        constexpr T eps = static_cast<T>(0.0000001);

        const Vec2<T> d = rhs.origin - lhs.origin;
        if (almostZero(d, eps))
        {
            return { lhs.origin, lhs.origin };
        }

        const Vec2<T> dnorm = (rhs.origin - lhs.origin).normalized();
        const Vec2<T> onLhs = lhs.origin + dnorm * lhs.radius;
        if (intersect(lhs, rhs))
        {
            return { onLhs, onLhs };
        }
        
        return { onLhs, rhs.origin - dnorm * rhs.radius };
    }
}

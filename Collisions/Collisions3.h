#pragma once

#include "../Shapes3.h"

namespace ls
{
    template <typename T>
    bool intersect(const ls::Vec3<T>& a, const ls::Box3<T>& b)
    {
        if (a.x < b.min.x) return false;
        if (a.y < b.min.y) return false;
        if (a.z < b.min.z) return false;
        if (a.x > b.max.x) return false;
        if (a.y > b.max.y) return false;
        if (a.z > b.max.z) return false;

        return true;
    }

    template <typename T>
    bool intersect(const ls::Box3<T>& a, const ls::Vec3<T>& b)
    {
        return intersect(b, a);
    }

    template <typename T>
    bool intersect(const ls::Box3<T>& a, const ls::Box3<T>& b)
    {
        if (a.max.x < b.min.x) return false;
        if (a.max.y < b.min.y) return false;
        if (a.max.z < b.min.z) return false;
        if (a.min.x > b.max.x) return false;
        if (a.min.y > b.max.y) return false;
        if (a.min.z > b.max.z) return false;

        return true;
    }

    template <typename T>
    bool intersect(const ls::Sphere3<T>& a, const ls::Sphere3<T>& b)
    {
        const T radiusSum = a.radius + b.radius;
        return a.origin.distanceSquared(b.origin) <= radiusSum * radiusSum;
    }

    template <typename T>
    bool intersect(const ls::Cylinder3<T>& a, const ls::Cylinder3<T>& b)
    {
        if (a.baseOrigin.y > b.baseOrigin.y + b.height) return false;
        if (b.baseOrigin.y > a.baseOrigin.y + a.height) return false;

        const T dx = a.baseOrigin.x - b.baseOrigin.x;
        const T dy = a.baseOrigin.y - b.baseOrigin.y;
        const T radiusSum = a.radius + b.radius;
        
        return dx*dx + dy*dy <= radiusSum * radiusSum;
    }

    template <typename T>
    bool intersect(const ls::Sphere3<T>& a, const ls::Vec3<T>& b)
    {
        return a.origin.distanceSquared(b) <= a.radius * a.radius;
    }

    template <typename T>
    bool intersect(const ls::Vec3<T>& a, const ls::Sphere3<T>& b)
    {
        return intersect(b, a);
    }
}

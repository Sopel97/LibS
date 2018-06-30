#pragma once

namespace ls
{
    template <typename T>
    struct Angle2;
    using Angle2F = Angle2<float>;
    using Angle2D = Angle2<double>;

    template <typename T>
    struct Box2;
    using Box2F = Box2<float>;
    using Box2D = Box2<double>;
    using Box2I = Box2<int>;

    template <typename T>
    struct Box3;
    using Box3F = Box3<float>;
    using Box3D = Box3<double>;
    using Box3I = Box3<int>;

    template <typename T>
    struct Capsule3;
    using Capsule3F = Capsule3<float>;
    using Capsule3D = Capsule3<double>;

    template <typename T>
    struct Circle2;
    using Circle2F = Circle2<float>;
    using Circle2D = Circle2<double>;

    template <typename T>
    struct ConvexPolygon2;
    using ConvexPolygon2F = ConvexPolygon2<float>;
    using ConvexPolygon2D = ConvexPolygon2<double>;

    template <typename T>
    struct Cylinder3;
    using Cylinder3F = Cylinder3<float>;
    using Cylinder3D = Cylinder3<double>;

    template <typename T>
    struct Edge2;
    using Edge2F = Edge2<float>;
    using Edge2D = Edge2<double>;

    template <typename T>
    struct Edge3;
    using Edge3F = Edge3<float>;
    using Edge3D = Edge3<double>;

    template <typename T>
    struct Polyline2;
    using Polyline2F = Polyline2<float>;
    using Polyline2D = Polyline2<double>;

    template <typename T>
    struct Ray2;
    using Ray2F = Ray2<float>;
    using Ray2D = Ray2<double>;

    template <typename T>
    struct Ray3;
    using Ray3F = Ray3<float>;
    using Ray3D = Ray3<double>;

    template <typename T>
    struct Sphere3;
    using Sphere3F = Sphere3<float>;
    using Sphere3D = Sphere3<double>;

    template <typename T>
    struct Plane3;
    using Plane3F = Plane3<float>;
    using Plane3D = Plane3<double>;

    template <typename T>
    struct Frustum3;
    using Frustum3F = Frustum3<float>;
    using Frustum3D = Frustum3<double>;

    template <typename T>
    struct Triangle2;
    using Triangle2F = Triangle2<float>;
    using Triangle2D = Triangle2<double>;

    template <typename T>
    struct Triangle3;
    using Triangle3F = Triangle3<float>;
    using Triangle3D = Triangle3<double>;

    template <typename T>
    struct Vec2;
    using Vec2F = Vec2<float>;
    using Vec2D = Vec2<double>;
    using Vec2I = Vec2<int>;

    template <typename T>
    struct Vec3;
    using Vec3F = Vec3<float>;
    using Vec3D = Vec3<double>;
    using Vec3I = Vec3<int>;

    template <typename T>
    struct Vec4;
    using Vec4F = Vec4<float>;
    using Vec4D = Vec4<double>;
    using Vec4I = Vec4<int>;

    template <typename T, int DimV>
    struct Vec;

    template <typename T>
    struct Vec<T, 1>
    {
        using Type = T;
    };
    template <typename T>
    struct Vec<T, 2>
    {
        using Type = Vec2<T>;
    };
    template <typename T>
    struct Vec<T, 3>
    {
        using Type = Vec3<T>;
    };
    template <typename T>
    struct Vec<T, 4>
    {
        using Type = Vec4<T>;
    };

    template <typename T, int DimV>
    using VecT = typename Vec<T, DimV>::Type;
}

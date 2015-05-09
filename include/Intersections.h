#ifndef INTERSECTIONS_H_INCLUDED
#define INTERSECTIONS_H_INCLUDED

class Intersections
{
public:

    template <class Shape>
    class LinearSweep //may be better to move it to Geo:: rather than leaving it in Intersections::
    {
    public:
        using VectorType = typename Shape::RespectiveVectorType;
        using ShapeType = Shape;

        LinearSweep(const Shape& shape, const VectorType& velocity);
        //don't know if enabling this is a good idea since initilalShape and finalShape may differ in rotation.
        //LinearSweep(const Shape& initialShape, const Shape& finalShape, double deltaTime);
        const Shape& initialShape() const;
        const VectorType& velocity() const;

        void setVelocity(const VectorType& newVelocity);
        void setInitialShape(const Shape& newInitialShape);

        Shape after(double time) const;

    private:
        const Shape* m_initialShape;
        VectorType m_velocity;
    };

    template <class SweepA, class SweepB>
    class SweepCollision
    {
    public:
        SweepCollision(const SweepA& sweepA, const SweepB& sweepB, double time);
        SweepCollision(SweepA&& sweepA, SweepB&& sweepB, double time);

        double timeTillCollision() const;
        const SweepA& firstShapeSweep() const;
        const SweepB& secondShapeSweep() const;

        typename SweepA::ShapeType firstShapeOnCollisionTime() const;
        typename SweepB::ShapeType secondShapeOnCollisionTime() const;

        bool anyCollision() const;

    private:
        const SweepA m_firstShapeSweep;
        const SweepB m_secondShapeSweep;
        double m_timeTillCollision; //storing all collisions may not be needed. THis will the closest one in time
    };


    //WARNING: Some intersections are not defined. Those will always return false!!!

    //2D shapes
    //Vec2
    template <class T>
    static bool intersection(const Vec2<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool intersection(const Vec2<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const Vec2<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const Vec2<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Vec2<T>& b); // temporary

    //LineSegmnet
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool intersection(const LineSegment<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const LineSegment<T>& b, Vec2<T>& intersectionPoint);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Vec2<T>& b){return false;}

    //Rectangle
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Vec2<T>& b);

    //Circle
    template <class T>
    static bool intersection(const Circle<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool intersection(const Circle<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool intersection(const Circle<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const Circle<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const Circle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Circle<T>& a, const Vec2<T>& b);

    //Triangle
    template <class T>
    static bool intersection(const Triangle<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool intersection(const Triangle<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool intersection(const Triangle<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool intersection(const Triangle<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const Triangle<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const Triangle<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Triangle<T>& a, const Triangle<T>& b); //need to be redone better
    template <class T>
    static bool intersection(const Triangle<T>& a, const Vec2<T>& b);

    //Polygon
    template <class T>
    static bool intersection(const Polygon<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool intersection(const Polygon<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool intersection(const Polygon<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const Polygon<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const Polygon<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Polygon<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Polygon<T>& a, const Vec2<T>& b);

    //Polyline
    template <class T>
    static bool intersection(const Polyline<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool intersection(const Polyline<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool intersection(const Polyline<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool intersection(const Polyline<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const Polyline<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const Polyline<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Polyline<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Polyline<T>& a, const Vec2<T>& b){return false;}

    //Ray
    template <class T>
    static bool intersection(const Ray<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool intersection(const Ray<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool intersection(const Ray<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool intersection(const Ray<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool intersection(const Ray<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool intersection(const Ray<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Ray<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool intersection(const Ray<T>& a, const Vec2<T>& b){return false;}

    //3D shapes
    //Vec3

    //Cylinder
    template <class T>
    static bool intersection(const Cylinder<T>& a, const Cylinder<T>& b);

    //Cuboid
    template <class T>
    static bool intersection(const Cuboid<T>& a, const Cuboid<T>& b);


    //Meshes
    template <class T>
    static bool intersection(const Mesh<T>& a, const Mesh<T>& b);
    template <class T, class S>
    static bool intersection(const Mesh<T>& a, const S& b);
    template <class T, class S>
    static bool intersection(const S& a, const Mesh<T>& b);

private:
    Intersections() {}
};
#include "../src/Intersections.cpp"

#endif // INTERSECTIONS_H_INCLUDED

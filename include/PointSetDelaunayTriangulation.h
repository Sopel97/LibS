#ifndef POINTSETDELAUNAYTRIANGULATION_H
#define POINTSETDELAUNAYTRIANGULATION_H

/*
    Most of the code is from
    https://github.com/ironwallaby/delaunay/blob/master/delaunay.js
*/

template <class T, class NodeType>
class PointSetDelaunayTriangulation : public Triangulation<T, NodeType>
{
public:
    PointSetDelaunayTriangulation(const std::vector<Vec2<T>>& points);
    PointSetDelaunayTriangulation(std::vector<Vec2<T>>&& points);

    virtual ~PointSetDelaunayTriangulation(){}

    virtual void calculate();

    virtual size_t numberOfPoints() const;
    virtual const std::vector<Vec2<T>>& points() const;
    virtual const Vec2<T>& point(size_t i) const;

protected:
    struct CircumCircle
    {
        size_t i; //indices of the vertices of the triangle
        size_t j;
        size_t k;
        Circle<T> circumcircle; //during computation stored radius will be squared to avoid unnecessary sqrt
    };
    std::vector<Vec2<T>> m_points;

    Triangle<T> superTriangle(const std::vector<Vec2<T>>& vertices) const;
    CircumCircle circumcircle(const std::vector<Vec2<T>>& vertices, size_t i, size_t j, size_t k) const;
};

typedef PointSetDelaunayTriangulation<double> PointSetDelaunayTriangulationD;
typedef PointSetDelaunayTriangulation<float> PointSetDelaunayTriangulationF;
typedef PointSetDelaunayTriangulation<int> PointSetDelaunayTriangulationI;

#include "../src/PointSetDelaunayTriangulation.cpp"

#endif // POINTSETDELAUNAYTRIANGULATION_H

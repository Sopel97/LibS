#ifndef POLYGONTRIANGULATION_H
#define POLYGONTRIANGULATION_H

/*
    most of the code is from
    http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml
*/

template <class T, class NodeType>
class PolygonTriangulation : public Triangulation<T, NodeType>
{
public:
    PolygonTriangulation(const Polygon<T>& polygon);
    PolygonTriangulation(Polygon<T>&& polygon);

    virtual ~PolygonTriangulation(){}

    virtual void calculate();

    const Polygon<T>& polygon() const;

    virtual size_t numberOfPoints() const;
    virtual const std::vector<Vec2<T>>& points() const;
    virtual const Vec2<T>& point(size_t i) const;

protected:
    Polygon<T> m_polygon;

    bool snip(size_t u, size_t v, size_t w, size_t n, const std::vector<size_t>& V);
};

typedef PolygonTriangulation<double> PolygonTriangulationD;
typedef PolygonTriangulation<float> PolygonTriangulationF;
typedef PolygonTriangulation<int> PolygonTriangulationI;

#include "../src/PolygonTriangulation.cpp"

#endif // POLYGONTRIANGULATION_H

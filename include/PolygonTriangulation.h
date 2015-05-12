#ifndef POLYGONTRIANGULATION_H
#define POLYGONTRIANGULATION_H

/*
    most of the code is from
    http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml
*/

template <class T>
class PolygonTriangulation
{
public:
    PolygonTriangulation(const Polygon<T>& polygon);
    PolygonTriangulation(Polygon<T>&& polygon);

    void calculate();

    const Mesh2<Triangle<T>>& result() const;
    const Polygon<T>& polygon() const;

protected:
    Polygon<T> m_polygon;
    Mesh2<Triangle<T>> m_result;

    bool snip(size_t u, size_t v, size_t w, size_t n, const std::vector<size_t>& V);
};

#include "../src/PolygonTriangulation.cpp"

#endif // POLYGONTRIANGULATION_H

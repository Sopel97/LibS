#ifndef POLYGONTRIANGULATION_H
#define POLYGONTRIANGULATION_H

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
};

#include "../src/PolygonTriangulation.cpp"

#endif // POLYGONTRIANGULATION_H

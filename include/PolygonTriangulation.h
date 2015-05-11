#ifndef POLYGONTRIANGULATION_H
#define POLYGONTRIANGULATION_H

template <class T>
class PolygonTriangulation
{
public:
    PolygonTriangulation(const Polygon<T>& polygon);
    void calculate();

    const Mesh2<Triangle<T>>& result() const;
    Mesh2<Triangle<T>>& result();
    Mesh2<Triangle<T>>&& releaseResult();

protected:
    const Polygon<T>& m_polygon; //only a reference, may be changed later
    Mesh2<Triangle<T>> m_result;
};

#include "../src/PolygonTriangulation.cpp"

#endif // POLYGONTRIANGULATION_H

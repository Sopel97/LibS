#ifndef POLYGONTRIANGULATION_H
#define POLYGONTRIANGULATION_H

/*
    most of the code is from
    http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml
*/

//TODO: make Triangulation class that contains:
//          - array of vertices and set of connections between them (as mapping of indices (size_t, size_t))
//          - resulting mesh of triangles (replacing current m_result)
//
//      And make it the base class for all Triangulations.
//
//      This will allow for easy constrution of graphs based on triagulation
//
//      Add a member that indicates whether the Triangulation is completed or not.
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

typedef PolygonTriangulation<double> PolygonTriangulationD;
typedef PolygonTriangulation<float> PolygonTriangulationF;
typedef PolygonTriangulation<int> PolygonTriangulationI;

#include "../src/PolygonTriangulation.cpp"

#endif // POLYGONTRIANGULATION_H

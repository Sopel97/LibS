#ifndef TRIANGULATION_H
#define TRIANGULATION_H

//NOTE: All triangulations are being computed when created.
template <class T, class NodeType>
class Triangulation
{
public:
    struct TriangleInd //stores only indices
    {
        size_t i;
        size_t j;
        size_t k;
    };

    struct EdgeInd
    {
        size_t i;
        size_t j;

        EdgeInd(size_t a, size_t b)
        {
            if(a > b)
            {
                i = a;
                j = b;
            }
            else
            {
                i = b;
                j = a;
            }
        }

        bool operator==(const EdgeInd& other) const
        {
            return ((i == other.i) && (j == other.j)); //since v1 and v2 are sorted
        }
        bool operator<(const EdgeInd& other) const
        {
            return ((i < other.i) || ((i == other.i) && (j < other.j)));
        }
    };

    Triangulation();

    virtual ~Triangulation();

    const Mesh2<Triangle<T>>& triangleMesh() const;
    const std::set<EdgeInd>& connections() const;
    const std::vector<TriangleInd>& triangles() const;

    virtual size_t numberOfPoints() const = 0;
    virtual const std::vector<Vec2<T>>& points() const = 0;
    virtual const Vec2<T>& point(size_t i) const = 0;

    bool isCompleted() const;

protected:
    Mesh2<Triangle<T>> m_triangleMesh;
    std::set<EdgeInd> m_connections;
    std::vector<TriangleInd> m_triangles;

    bool m_isCompleted;
};

typedef Triangulation<double> TriangulationD;
typedef Triangulation<float> TriangulationF;
typedef Triangulation<int> TriangulationI;

#include "../src/Triangulation.cpp"

#endif // TRIANGULATION_H

#ifndef TRIANGULATION_H
#define TRIANGULATION_H

template <class T>
class Triangulation
{
public:
    struct Edge
    {
        size_t v1;
        size_t v2;

        bool operator==(const Edge& other) const
        {
            return ((v1 == other.v2) || (v2 == other.v1));
        }
        bool operator<(const Edge& other) const
        {
            return ((v1 < other.v1) || ((v1 == v2) && (v2 < other.v2)));
        }
    };

    Triangulation();

    virtual void calculate() = 0;

    const Mesh2<Triangle<T>>& triangles() const;
    const std::set<Edge> connections() const;

    bool isCompleted() const;

protected:
    Mesh2<Triangle<T>> m_triangles;
    std::set<Edge> m_connections; //vertex to vertex connection

    bool m_isCompleted;
};

typedef Triangulation<double> TriangulationD;
typedef Triangulation<float> TriangulationF;
typedef Triangulation<int> TriangulationI;

#include "../src/Triangulation.cpp"

#endif // TRIANGULATION_H

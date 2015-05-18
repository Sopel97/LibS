#ifndef TRIANGULATION_H
#define TRIANGULATION_H

template <class T, class NodeType>
class Triangulation
{
public:
    struct Edge
    {
        size_t v1;
        size_t v2;

        Edge(size_t a, size_t b)
        {
            if(a > b)
            {
                v1 = a;
                v2 = b;
            }
            else
            {
                v1 = b;
                v2 = a;
            }
        }

        bool operator==(const Edge& other) const
        {
            return ((v1 == other.v1) && (v2 == other.v2)); //since v1 and v2 are sorted
        }
        bool operator<(const Edge& other) const
        {
            return ((v1 < other.v1) || ((v1 == other.v1) && (v2 < other.v2)));
        }
    };

    Triangulation();

    virtual ~Triangulation();

    virtual void calculate() = 0;

    const Mesh2<Triangle<T>>& triangles() const;
    const std::set<Edge>& connections() const;
    const std::vector<NodeType*>& graph() const;

    void createGraphFromConnections();

    virtual size_t numberOfPoints() const = 0;
    virtual const std::vector<Vec2<T>>& points() const = 0;
    virtual const Vec2<T>& point(size_t i) const = 0;

    bool isCompleted() const;

protected:
    Mesh2<Triangle<T>> m_triangles;
    std::set<Edge> m_connections; //vertex to vertex connection
    std::vector<NodeType*> m_graph;

    bool m_isCompleted;
};

typedef Triangulation<double> TriangulationD;
typedef Triangulation<float> TriangulationF;
typedef Triangulation<int> TriangulationI;

#include "../src/Triangulation.cpp"

#endif // TRIANGULATION_H

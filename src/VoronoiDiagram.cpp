template <class T>
VoronoiDiagram<T>::VoronoiDiagram(const std::vector<Vec2<T>>& points)
{
    calculate(PointSetDelaunayTriangulation<T>(points));
}
template <class T>
VoronoiDiagram<T>::VoronoiDiagram(std::vector<Vec2<T>>&& points)
{
    calculate(PointSetDelaunayTriangulation<T>(std::move(points)));
}
template <class T>
VoronoiDiagram<T>::VoronoiDiagram(const PointSetDelaunayTriangulation<T>& triangulation)
{
    calculate(triangulation);
}

template <class T>
void VoronoiDiagram<T>::calculate(const PointSetDelaunayTriangulation<T>& triangulation)
{
    class AngleSort
    {
        // From http://stackoverflow.com/questions/7774241/sort-points-by-angle-from-given-axis
        // by @Ben Voigt

    public:
        AngleSort(const Vec2<T>& origin, const Vec2<T>& reference) : m_origin(origin), m_dreference(reference - origin) {}
        bool operator()(const Vec2<T>& a, const Vec2<T>& b) const
        {
            const Vec2<T> da = a - m_origin;
            const Vec2<T> db = b - m_origin;
            const double detb = m_dreference.cross(db);

            // nothing is less than zero degrees
            if(std::abs(detb) <= EPSILON && db.dot(m_dreference) >= 0) return false;

            const double deta = m_dreference.cross(da);

            // zero degrees is less than anything else
            if(std::abs(deta) <= EPSILON && da.dot(m_dreference) >= 0) return true;

            if(deta * detb >= 0)
            {
                // both on same side of reference, compare to each other
                return da.cross(db) > 0;
            }

            // TODO: polygons for which this point is reached when sorting should be excluded from voronoi diagram since they extend to infinity.
            //       somehow make it give such information in return

            // vectors "less than" zero degrees are actually large, near 2 pi
            return deta > 0;
        }
    protected:
        Vec2<T> m_origin;
        Vec2<T> m_dreference;
    };

    m_points.clear();
    m_polygons.clear();
    m_connections.clear();

    std::vector<std::vector<Vec2<T>>> polygons; // vertices of polygon around vertex i (for i being index of every point in m_points)
    // for vertices on the boundary it will form only part of the polygon.
    // These need to be recognized and either extended to infinity or removed and replaced by two rays (the second one is on todo list)

    const auto& triangles = triangulation.triangles();
    const auto& delaunayPoints = triangulation.points();

    m_points.reserve(triangles.size);
    polygons.resize(triangles.size());

    for(const auto& triangle : triangles)
    {
        const Vec2<T>& currentCircumcircle = triangle.circumcircle.origin;

        m_points.push_back(currentCircumcircle);

        polygons[triangle.i].push_back(currentCircumcircle);
        polygons[triangle.j].push_back(currentCircumcircle);
        polygons[triangle.k].push_back(currentCircumcircle);
    }

    size_t i = 0;
    for(auto& polygonUnorderedPoints : polygons)
    {
        std::sort(polygonUnorderedPoints.begin() + 1, polygonUnorderedPoints.end(), AngleSort(delaunayPoints[i++], polygonUnorderedPoints.front()));
    }

    // TODO: some of the polygons are still to be removed

    m_polygons.reserve(polygons.size());
    for(auto& polygon : polygons)
    {
        size_t size = polygon.size();
        for(size_t i = 0; i < size; ++i)
        {
            m_connections.insert(typename Triangulation<T>::EdgeInd(polygon[i], polygon[(i + 1) % size]));
        }

        m_polygons.emplace_back(std::move(polygon));
    }
}

template <class T, class NodeType>
PolygonTriangulation<T, NodeType>::PolygonTriangulation(const Polygon<T>& polygon) :
    Triangulation<T>::Triangulation(),
    m_polygon(polygon)
{

}
template <class T, class NodeType>
PolygonTriangulation<T, NodeType>::PolygonTriangulation(Polygon<T>&& polygon) :
    Triangulation<T>::Triangulation(),
    m_polygon(std::move(polygon))
{

}
template <class T, class NodeType>
void PolygonTriangulation<T, NodeType>::calculate()
{
    //for convex shape
    if(this->m_isCompleted) return;

    this->m_triangles.elements.clear();
    this->m_connections.clear();

    if(m_polygon.size() < 3) return;
    if(m_polygon.isConvex())
    {
        size_t numberOfVertices = m_polygon.size();
        this->m_triangles.elements.reserve(numberOfVertices - 2);
        const Vec2<T>& firstVertex = m_polygon.vertices[0];
        for(size_t i = 2; i < numberOfVertices; ++i)
        {
            this->m_triangles.add(Triangle<T>(firstVertex, m_polygon.vertices[i - 1], m_polygon.vertices[i]));
        }
    }
    else
    {
        /* allocate and initialize list of Vertices in polygon */

        size_t n = m_polygon.size();
        if(n < 3) return;
        std::vector<size_t> V(n);

        /* we want a counter-clockwise polygon in V */

        if(m_polygon.signedArea() > 0.0)
            for(size_t v = 0; v < n; ++v) V[v] = v;
        else
            for(size_t v = 0; v < n; ++v) V[v] = (n - 1) - v;

        size_t nv = n;

        /*  remove nv-2 Vertices, creating 1 triangle every time */
        int count = 2 * nv; /* error detection */ //probably can be a size_t

        for(size_t m = 0, v = nv - 1; nv > 2;)
        {
            /* if we loop, it is probably a non-simple polygon */
            if(--count <= 0)
            {
                //** Triangulate: ERROR - probable bad polygon!
                return;
            }

            /* three consecutive vertices in current polygon, <u,v,w> */
            size_t u = v;
            if(nv <= u) u = 0;      /* previous */
            v = u + 1;
            if(nv <= v) v = 0;      /* new v    */
            size_t w = v + 1;
            if(nv <= w) w = 0;      /* next     */

            if(snip(u, v, w, nv, V))
            {
                size_t a, b, c, s, t;

                /* true names of the vertices */
                a = V[u]; b = V[v]; c = V[w];

                /* output Triangle */
                this->m_triangles.add(Triangle<T>(m_polygon.vertices[a], m_polygon.vertices[b], m_polygon.vertices[c]));
                this->m_connections.insert(typename Triangulation<T, NodeType>::Edge(a, b));
                this->m_connections.insert(typename Triangulation<T, NodeType>::Edge(b, c));
                this->m_connections.insert(typename Triangulation<T, NodeType>::Edge(c, a));
                ++m;

                /* remove v from remaining polygon */
                for(s = v, t = v + 1; t < nv; ++s, ++t) V[s] = V[t]; --nv;

                /* resest error detection counter */
                count = 2 * nv;
            }
        }
    }

    this->createGraphFromConnections();
    this->m_isCompleted = true;
}

template <class T, class NodeType>
const Polygon<T>& PolygonTriangulation<T, NodeType>::polygon() const
{
    return m_polygon;
}
template <class T, class NodeType>
size_t PolygonTriangulation<T, NodeType>::numberOfPoints() const
{
    return m_polygon.size();
}
template <class T, class NodeType>
const std::vector<Vec2<T>>& PolygonTriangulation<T, NodeType>::points() const
{
    return m_polygon.vertices;
}
template <class T, class NodeType>
const Vec2<T>& PolygonTriangulation<T, NodeType>::point(size_t i) const
{
    return m_polygon.vertices[i];
}

template <class T, class NodeType>
bool PolygonTriangulation<T, NodeType>::snip(size_t u, size_t v, size_t w, size_t n, const std::vector<size_t>& V)
{
    const Vec2<T>& a = m_polygon.vertices[V[u]];
    const Vec2<T>& b = m_polygon.vertices[V[v]];
    const Vec2<T>& c = m_polygon.vertices[V[w]];
    Triangle<T> triangle(a, b, c);

    if(EPSILON > (((b.x - a.x) * (c.y - a.y)) - ((b.y - a.y) * (c.x - a.x)))) return false; // i misinterpreted it as an area function earlier

    for(size_t p = 0; p < n; p++)
    {
        if((p == u) || (p == v) || (p == w)) continue;
        if(triangle.intersects(m_polygon.vertices[V[p]])) return false;
    }

    return true;
}

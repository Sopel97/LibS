template <class T>
PolygonTriangulation<T>::PolygonTriangulation(const Polygon<T>& polygon) :
    m_polygon(polygon)
{

}
template <class T>
PolygonTriangulation<T>::PolygonTriangulation(Polygon<T>&& polygon) :
    m_polygon(std::move(polygon))
{

}
template <class T>
void PolygonTriangulation<T>::calculate()
{
    //for convex shape
    if(m_polygon.size() < 3) return;
    if(m_polygon.isConvex())
    {
        size_t numberOfVertices = m_polygon.size();
        m_result.elements.reserve(numberOfVertices - 2);
        const Vec2<T>& firstVertex = m_polygon.vertices[0];
        for(size_t i = 2; i < numberOfVertices; ++i)
        {
            m_result.add(Triangle<T>(firstVertex, m_polygon.vertices[i - 1], m_polygon.vertices[i]));
        }
    }
    else
    {
        /* allocate and initialize list of Vertices in polygon */

        size_t n = m_polygon.size();
        if(n < 3) return;
        std::vector<size_t> V;

        /* we want a counter-clockwise polygon in V */

        if (m_polygon.signedArea() > 0.0)
            for(size_t v = 0; v < n; ++v) V[v] = v;
        else
            for(size_t v = 0; v < n; ++v) V[v] = (n - 1) - v;

        size_t nv = n;

        /*  remove nv-2 Vertices, creating 1 triangle every time */
        int count = 2 * nv; /* error detection */ //probably can be a size_t

        for(size_t m = 0, v = nv - 1; nv > 2; )
        {
            /* if we loop, it is probably a non-simple polygon */
            if (--count <= 0)
            {
                //** Triangulate: ERROR - probable bad polygon!
                return;
            }

            /* three consecutive vertices in current polygon, <u,v,w> */
            size_t u = v  ;
            if (nv <= u) u = 0;     /* previous */
            v = u + 1;
            if (nv <= v) v = 0;     /* new v    */
            size_t w = v + 1;
            if (nv <= w) w = 0;     /* next     */

            if ( snip(u, v, w, nv, V) )
            {
                size_t a, b, c, s, t;

                /* true names of the vertices */
                a = V[u]; b = V[v]; c = V[w];

                /* output Triangle */
                m_result.add(Triangle<T>(m_polygon.vertices[a], m_polygon.vertices[b], m_polygon.vertices[c]));

                ++m;

                /* remove v from remaining polygon */
                for(s = v, t = v + 1; t < nv; ++s, ++t) V[s] = V[t]; --nv;

                /* resest error detection counter */
                count = 2 * nv;
            }
        }

    }
}

template <class T>
const Mesh2<Triangle<T>>& PolygonTriangulation<T>::result() const
{
    return m_result;
}
template <class T>
const Polygon<T>& PolygonTriangulation<T>::polygon() const
{
    return m_polygon;
}

template <class T>
bool PolygonTriangulation<T>::snip(size_t u, size_t v, size_t w, size_t n, const std::vector<size_t>& V)
{
    Triangle<T> triangle(m_polygon.vertices[V[u]], m_polygon.vertices[V[v]], m_polygon.vertices[V[w]]);

    if(EPSILON > triangle.area()) return false; //(((Bx - Ax) * (Cy - Ay)) - ((By - Ay) * (Cx - Ax))) ) return false;

    for (size_t p = 0; p < n; p++)
    {
        if( (p == u) || (p == v) || (p == w) ) continue;
        if (triangle.intersects(m_polygon.vertices[V[p]])) return false;
    }

    return true;
}

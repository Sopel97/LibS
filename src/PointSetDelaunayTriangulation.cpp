template <class T, class NodeType>
PointSetDelaunayTriangulation<T, NodeType>::PointSetDelaunayTriangulation(const std::vector<Vec2<T>>& points) :
    m_points(points)
{

}
template <class T, class NodeType>
PointSetDelaunayTriangulation<T, NodeType>::PointSetDelaunayTriangulation(std::vector<Vec2<T>>&& points) :
    m_points(std::move(points))
{

}

template <class T, class NodeType>
void PointSetDelaunayTriangulation<T, NodeType>::calculate()
{
    std::vector<Vec2<T>> vertices = m_points;

    size_t n = vertices.size();
    //i, j, indices, st, open, closed, edges, dx, dy, a, b, c;

    /* Bail if there aren't enough vertices to form any triangles. */
    if(n < 3) return;

    /* ?????
          if(key)
            for(i = n; i--; )
              vertices[i] = vertices[i][key];
    */

    /* Make an array of indices into the vertex array, sorted by the
     * vertices' x-position. */
    std::vector<size_t> indices;
    indices.reserve(n);

    for(size_t i = 0; i < n; ++i)
    {
        indices.push_back(i);
    }

    std::sort(indices.begin(), indices.end(), [&vertices](const size_t& i, const size_t& j) -> bool {return vertices[i].x > vertices[j].x;}); //this order may be inversed, don't really know

    /* Next, find the vertices of the supertriangle (which contains all other
     * triangles), and append them onto the end of a (copy of) the vertex
     * array. */
    Triangle<T> st = superTriangle(vertices);
    vertices.push_back(st.vertices[0]);
    vertices.push_back(st.vertices[1]);
    vertices.push_back(st.vertices[2]);

    /* Initialize the open list (containing the supertriangle and nothing
     * else) and the closed list (which is empty since we havn't processed
     * any triangles yet). */
    std::vector<CircumCircle> open({circumcircle(vertices, n + 0, n + 1, n + 2)});
    std::vector<CircumCircle> closed;
    std::set<typename Triangulation<T, NodeType>::Edge> edges;

    /* Incrementally add each vertex to the mesh. */
    for(int i = indices.size() - 1; i >= 0; --i, edges.clear())//, edges.length = 0)
    {
        size_t c = indices[i];

        /* For each open triangle, check to see if the current point is
         * inside it's circumcircle. If it is, remove the triangle and add
         * it's edges to an edge list. */
        for(int j = open.size() - 1; j >= 0; --j)
        {
            /* If this point is to the right of this triangle's circumcircle,
             * then this triangle should never get checked again. Remove it
             * from the open list, add it to the closed list, and skip. */
            T dx = vertices[c].x - open[j].circle.origin.x;
            if(dx > 0.0 && dx * dx > open[j].circle.radius)
            {
                closed.push_back(open[j]);
                open.erase(open.begin() + j);
                continue;
            }

            /* If we're outside the circumcircle, skip this triangle. */
            T dy = vertices[c].y - open[j].circle.origin.y;
            if(dx * dx + dy * dy > open[j].circle.radius)
            {
                continue;
            }

            /* Remove the triangle and add it's edges to the edge list. */
            edges.insert(typename Triangulation<T, NodeType>::Edge(open[j].i, open[j].j));
            edges.insert(typename Triangulation<T, NodeType>::Edge(open[j].j, open[j].k));
            edges.insert(typename Triangulation<T, NodeType>::Edge(open[j].k, open[j].i));

            open.erase(open.begin() + j);
        }

        /* Remove any doubled edges. */
        //dedup(edges); this is done automatically since we store edges in a set

        /* Add a new triangle for each edge. */
        for(const auto& edge : edges)
        {
            open.push_back(circumcircle(vertices, edge.v1, edge.v2, c));
        }
    }

    /* Copy any remaining open triangles to the closed list, and then
     * remove any triangles that share a vertex with the supertriangle,
     * building a list of triplets that represent triangles. */
    for(int i = open.size() - 1; i >= 0; --i)
        closed.push_back(open[i]);
    open.clear();
    this->m_connections.clear();
    for(int i = closed.size() - 1; i >= 0; --i)
    {
        if(closed[i].i < n && closed[i].j < n && closed[i].k < n)
        {
            this->m_triangles.add(Triangle<T>(vertices[closed[i].i], vertices[closed[i].j], vertices[closed[i].k]));
            this->m_connections.insert(typename Triangulation<T, NodeType>::Edge(closed[i].i, closed[i].j));
            this->m_connections.insert(typename Triangulation<T, NodeType>::Edge(closed[i].j, closed[i].k));
            this->m_connections.insert(typename Triangulation<T, NodeType>::Edge(closed[i].k, closed[i].i));
        }
    }
}
/* ???
contains: function(tri, p)
{
    // Bounding box test first, for quick rejections.
    if((p[0] < tri[0][0] && p[0] < tri[1][0] && p[0] < tri[2][0]) ||
            (p[0] > tri[0][0] && p[0] > tri[1][0] && p[0] > tri[2][0]) ||
            (p[1] < tri[0][1] && p[1] < tri[1][1] && p[1] < tri[2][1]) ||
            (p[1] > tri[0][1] && p[1] > tri[1][1] && p[1] > tri[2][1]))
        return null;

    var a = tri[1][0] - tri[0][0],
        b = tri[2][0] - tri[0][0],
        c = tri[1][1] - tri[0][1],
        d = tri[2][1] - tri[0][1],
        i = a * d - b * c;

    // Degenerate tri.
    if(i == = 0.0)
        return null;

    var u = (d * (p[0] - tri[0][0]) - b * (p[1] - tri[0][1])) / i,
        v = (a * (p[1] - tri[0][1]) - c * (p[0] - tri[0][0])) / i;

    // If we're outside the tri, fail.
    if(u < 0.0 || v < 0.0 || (u + v) > 1.0)
        return null;

    return [u, v];
}
*/

template <class T, class NodeType>
size_t PointSetDelaunayTriangulation<T, NodeType>::numberOfPoints() const
{
    return m_points.size();
}
template <class T, class NodeType>
const std::vector<Vec2<T>>& PointSetDelaunayTriangulation<T, NodeType>::points() const
{
    return m_points;
}
template <class T, class NodeType>
const Vec2<T>& PointSetDelaunayTriangulation<T, NodeType>::point(size_t i) const
{
    return m_points[i];
}



template <class T, class NodeType>
Triangle<T> PointSetDelaunayTriangulation<T, NodeType>::superTriangle(const std::vector<Vec2<T>>& vertices)
{
    T xmin = std::numeric_limits<T>::max();
    T ymin = std::numeric_limits<T>::max();
    T xmax = std::numeric_limits<T>::min();
    T ymax = std::numeric_limits<T>::min();

    for(int i = vertices.size() - 1; i >= 0; --i)
    {
        if(vertices[i].x < xmin) xmin = vertices[i].x;
        if(vertices[i].x > xmax) xmax = vertices[i].x;
        if(vertices[i].y < ymin) ymin = vertices[i].y;
        if(vertices[i].y > ymax) ymax = vertices[i].y;
    }

    T dx = xmax - xmin;
    T dy = ymax - ymin;
    T dmax = std::max(dx, dy);
    T xmid = xmin + dx * 0.5;
    T ymid = ymin + dy * 0.5;

    return Triangle<T>(Vec2<T>(xmid - 20.0 * dmax, ymid -      dmax),
                       Vec2<T>(xmid              , ymid + 20.0 * dmax),
                       Vec2<T>(xmid + 20.0 * dmax, ymid -      dmax));
}

template <class T, class NodeType>
typename PointSetDelaunayTriangulation<T, NodeType>::CircumCircle PointSetDelaunayTriangulation<T, NodeType>::circumcircle(const std::vector<Vec2<T>>& vertices, size_t i, size_t j, size_t k)
{
    T x1 = vertices[i].x;
    T y1 = vertices[i].y;
    T x2 = vertices[j].x;
    T y2 = vertices[j].y;
    T x3 = vertices[k].x;
    T y3 = vertices[k].y;
    T absy1y2 = std::abs(y1 - y2);
    T absy2y3 = std::abs(y2 - y3);

    T xc, yc, m1, m2, mx1, mx2, my1, my2;

    /* Check for coincident points */
    if(absy1y2 < EPSILON && absy2y3 < EPSILON)
    {
        //coincident points
    }

    if(absy1y2 < EPSILON)
    {
        m2  = -((x3 - x2) / (y3 - y2));
        mx2 = (x2 + x3) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc  = (x2 + x1) / 2.0;
        yc  = m2 * (xc - mx2) + my2;
    }
    else if(absy2y3 < EPSILON)
    {
        m1  = -((x2 - x1) / (y2 - y1));
        mx1 = (x1 + x2) / 2.0;
        my1 = (y1 + y2) / 2.0;
        xc  = (x3 + x2) / 2.0;
        yc  = m1 * (xc - mx1) + my1;
    }
    else
    {
        m1  = -((x2 - x1) / (y2 - y1));
        m2  = -((x3 - x2) / (y3 - y2));
        mx1 = (x1 + x2) / 2.0;
        mx2 = (x2 + x3) / 2.0;
        my1 = (y1 + y2) / 2.0;
        my2 = (y2 + y3) / 2.0;
        xc  = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        yc  = (absy1y2 > absy2y3) ?
              m1 * (xc - mx1) + my1 :
              m2 * (xc - mx2) + my2;
    }

    T dx = x2 - xc;
    T dy = y2 - yc;

    return CircumCircle {i, j, k, Circle<T>(Vec2<T>(xc, yc), dx * dx + dy * dy)}; //radius squared.
}

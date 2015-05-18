template <class T, class NodeType>
PointSetDelaunayTriangulation<T, NodeType>::PointSetDelaunayTriangulation(const std::vector<Vec2<T>>& points) :
    m_points(points)
{
    calculate();
}
template <class T, class NodeType>
PointSetDelaunayTriangulation<T, NodeType>::PointSetDelaunayTriangulation(std::vector<Vec2<T>>&& points) :
    m_points(std::move(points))
{
    calculate();
}

template <class T, class NodeType>
void PointSetDelaunayTriangulation<T, NodeType>::calculate()
{
    this->m_connections.clear();
    this->m_triangleMesh.clear();
    this->m_triangles.clear();
    m_closedTriangles.clear();

    //copy of the vertices because we add 3 vertices later (supertriangle's vertices)
    std::vector<Vec2<T>> vertices = m_points;

    size_t n = vertices.size();

    // Bail if there aren't enough vertices to form any triangles.
    if(n < 3) return;

    // Make an array of indices into the vertex array, sorted ascending by the vertices' x-position.
    std::vector<size_t> indices;
    indices.reserve(n);

    for(size_t i = 0; i < n; ++i)
    {
        indices.push_back(i);
    }

    std::sort(indices.begin(), indices.end(), [&vertices](const size_t& i, const size_t& j) -> bool {return vertices[i].x < vertices[j].x;});

    // Next, find the vertices of the supertriangle (which contains all other
    // triangles), and append them onto the end of a (copy of) the vertex array
    Triangle<T> st = superTriangle(vertices);
    vertices.push_back(st.vertices[0]);
    vertices.push_back(st.vertices[1]);
    vertices.push_back(st.vertices[2]);

    // Initialize the open list (containing the supertriangle and nothing else)
    // and the closed list (which is empty since we havn't processed any triangles yet).
    // all triangles are stored as indices of vertices along with their circumcircles
    std::vector<CircumCircle> open({circumcircle(vertices, n + 0, n + 1, n + 2)});

    // Incrementally add each vertex to the mesh.
    for(size_t c : indices)
    {
        std::set<typename Triangulation<T, NodeType>::Edge> edges;

        // For each open triangle, check to see if the current point is
        // inside it's circumcircle. If it is, remove the triangle and add
        // it's edges to an edge list.
        for(int j = open.size() - 1; j >= 0; --j) //backward, so we don't have to adjust j after erasing circumcircles
        {
            // If this point is to the right of this triangle's circumcircle (that means we are past the circumcircle),
            // then this triangle should never get checked again.
            // Remove it from the open list, add it to the closed list, and skip.
            T dx = vertices[c].x - open[j].circumcircle.origin.x;
            if(dx > 0.0 && dx * dx > open[j].circumcircle.radius)
            {
                m_closedTriangles.push_back(open[j]);
                open.erase(open.begin() + j);
                continue;
            }

            // If we're outside the circumcircle, skip this triangle.
            T dy = vertices[c].y - open[j].circumcircle.origin.y;
            if(dx * dx + dy * dy > open[j].circumcircle.radius)
            {
                continue;
            }

            // If we are inside a circumference remove the triangle and add it's edges to the edge list.
            // All edges should be unique, that's automatically handled by a set.
            edges.insert(typename Triangulation<T, NodeType>::EdgeInd(open[j].i, open[j].j));
            edges.insert(typename Triangulation<T, NodeType>::EdgeInd(open[j].j, open[j].k));
            edges.insert(typename Triangulation<T, NodeType>::EdgeInd(open[j].k, open[j].i));

            open.erase(open.begin() + j);
        }

        /* Add a new triangle constructed from each edge and point indicated by c. */
        for(const auto& edge : edges)
        {
            open.push_back(circumcircle(vertices, edge.i, edge.j, c));
        }
    }

    // Copy any remaining open triangles to the closed list, and then
    for(const auto& triangle : open)
    {
        m_closedTriangles.push_back(triangle);
    }

    for(const auto& triangle : m_closedTriangles)
    {
        // onlt triangles that DO NOT share a vertex with the supertriangle (since they are outside point set)
        if(triangle.i < n && triangle.j < n && triangle.k < n)
        {
            this->m_triangleMesh.add(Triangle<T>(vertices[triangle.i], vertices[triangle.j], vertices[triangle.k]));

            this->m_connections.insert(typename Triangulation<T, NodeType>::EdgeInd(triangle.i, triangle.j));
            this->m_connections.insert(typename Triangulation<T, NodeType>::EdgeInd(triangle.j, triangle.k));
            this->m_connections.insert(typename Triangulation<T, NodeType>::EdgeInd(triangle.k, triangle.i));

            m_closedTriangles.push_back(triangle);

            this->m_triangles.push_back(typename Triangulation<T, NodeType>::TriangleInd(triangle.i, triangle.l, triangle.k));
        }
    }

    this->m_isCompleted = true;
}

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
const std::vector<typename PointSetDelaunayTriangulation<T, NodeType>::CircumCircle>& PointSetDelaunayTriangulation<T, NodeType>::closedTriangles() const
{
    return m_closedTriangles;
}


template <class T, class NodeType>
Triangle<T> PointSetDelaunayTriangulation<T, NodeType>::superTriangle(const std::vector<Vec2<T>>& vertices) const
{
    T xmin = std::numeric_limits<T>::max();
    T ymin = std::numeric_limits<T>::max();
    T xmax = std::numeric_limits<T>::min();
    T ymax = std::numeric_limits<T>::min();

    for(const auto& vertex : vertices)
    {
        if(vertex.x < xmin) xmin = vertex.x;
        if(vertex.x > xmax) xmax = vertex.x;
        if(vertex.y < ymin) ymin = vertex.y;
        if(vertex.y > ymax) ymax = vertex.y;
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
typename PointSetDelaunayTriangulation<T, NodeType>::CircumCircle PointSetDelaunayTriangulation<T, NodeType>::circumcircle(const std::vector<Vec2<T>>& vertices, size_t i, size_t j, size_t k) const
{
    T x1 = vertices[i].x;
    T y1 = vertices[i].y;
    T x2 = vertices[j].x;
    T y2 = vertices[j].y;
    T x3 = vertices[k].x;
    T y3 = vertices[k].y;
    T absy1y2 = std::abs(y1 - y2);
    T absy2y3 = std::abs(y2 - y3);

    T xc, yc;

    // Check for coincident points
    if(absy1y2 < EPSILON && absy2y3 < EPSILON)
    {
        //coincident points
        //TODO: something about that
    }

    if(absy1y2 < EPSILON)
    {
        T m2  = -((x3 - x2) / (y3 - y2));
        T mx2 = (x2 + x3) / 2.0;
        T my2 = (y2 + y3) / 2.0;

        xc  = (x2 + x1) / 2.0;
        yc  = m2 * (xc - mx2) + my2;
    }
    else if(absy2y3 < EPSILON)
    {
        T m1  = -((x2 - x1) / (y2 - y1));
        T mx1 = (x1 + x2) / 2.0;
        T my1 = (y1 + y2) / 2.0;

        xc  = (x3 + x2) / 2.0;
        yc  = m1 * (xc - mx1) + my1;
    }
    else
    {
        T m1  = -((x2 - x1) / (y2 - y1));
        T m2  = -((x3 - x2) / (y3 - y2));
        T mx1 = (x1 + x2) / 2.0;
        T mx2 = (x2 + x3) / 2.0;
        T my1 = (y1 + y2) / 2.0;
        T my2 = (y2 + y3) / 2.0;

        xc  = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
        yc  = (absy1y2 > absy2y3) ?
              m1 * (xc - mx1) + my1 :
              m2 * (xc - mx2) + my2;
    }

    T dx = x2 - xc;
    T dy = y2 - yc;

    return CircumCircle {i, j, k, Circle<T>(Vec2<T>(xc, yc), dx * dx + dy * dy)}; //radius squared.
}
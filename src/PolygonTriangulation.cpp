template <class T>
PolygonTriangulation<T>::PolygonTriangulation(const Polygon<T>& polygon) :
    m_polygon(polygon)
{

}
template <class T>
void PolygonTriangulation<T>::calculate()
{
    //for convex shape
    //if(m_polygon.isConvex()) ...

    size_t numberOfVertices = m_polygon.size();
    m_result.elements.reserve(numberOfVertices-2);
    const Vec2<T>& firstVertex = m_polygon.vertices[0];
    for(size_t i = 2; i < numberOfVertices; ++i)
    {
        m_result.add(Triangle<T>(firstVertex, m_polygon.vertices[i-1], m_polygon.vertices[i]));
    }
}

template <class T>
const Mesh2<Triangle<T>>& PolygonTriangulation<T>::result() const
{
    return m_result;
}
template <class T>
Mesh2<Triangle<T>>& PolygonTriangulation<T>::result()
{
    return m_result;
}
template <class T>
Mesh2<Triangle<T>>&& PolygonTriangulation<T>::releaseResult()
{
    return std::move(m_result);
}

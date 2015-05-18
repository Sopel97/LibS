template <class T, class NodeType>
Triangulation<T, NodeType>::Triangulation() :
    m_isCompleted(false)
{

}
template <class T, class NodeType>
Triangulation<T, NodeType>::~Triangulation()
{
}

template <class T, class NodeType>
const Mesh2<Triangle<T>>& Triangulation<T, NodeType>::triangleMesh() const
{
    return m_triangleMesh;
}
template <class T, class NodeType>
const std::set<typename Triangulation<T, NodeType>::EdgeInd>& Triangulation<T, NodeType>::connections() const
{
    return m_connections;
}
template <class T, class NodeType>
const std::vector<typename Triangulation<T, NodeType>::TriangleInd>& Triangulation<T, NodeType>::triangles() const
{
    return m_triangles;
}

template <class T, class NodeType>
bool Triangulation<T, NodeType>::isCompleted() const
{
    return m_isCompleted;
}

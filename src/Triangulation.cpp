template <class T, class NodeType>
Triangulation<T, NodeType>::Triangulation() :
    m_isCompleted(false)
{

}
template <class T, class NodeType>
Triangulation<T, NodeType>::~Triangulation()
{
    for(auto& node : m_graph)
    {
        delete node;
    }
}

template <class T, class NodeType>
const Mesh2<Triangle<T>>& Triangulation<T, NodeType>::triangles() const
{
    return m_triangles;
}
template <class T, class NodeType>
const std::set<typename Triangulation<T, NodeType>::Edge>& Triangulation<T, NodeType>::connections() const
{
    return m_connections;
}
template <class T, class NodeType>
const std::vector<NodeType*>& Triangulation<T, NodeType>::graph() const
{
    return m_graph;
}

template <class T, class NodeType>
void Triangulation<T, NodeType>::createGraphFromConnections()
{
    for(auto& node : m_graph) delete node;
    m_graph.clear();

    size_t pointCount = numberOfPoints();
    m_graph.reserve(pointCount);
    for(const auto& point : points())
    {
        m_graph.push_back(new NodeType(point));
    }

    for(const auto& connection : m_connections)
    {
        auto& node1 = m_graph[connection.v1];
        auto& node2 = m_graph[connection.v2];

        node1->addChild(node2);
        node2->addChild(node1);
    }
}

template <class T, class NodeType>
bool Triangulation<T, NodeType>::isCompleted() const
{
    return m_isCompleted;
}

template <class T>
RapidlyExploringRandomTree<T>::Node::Node(Node* parent, RapidlyExploringRandomTree<T>::Node* child) :
    m_position(0.0f, 0.0f),
    m_children {child},
           m_parent(parent)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(const Vec2<T>& position, Node* parent) :
    m_position(position),
    m_parent(parent)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(const Vec2<T>& position, Node* parent, RapidlyExploringRandomTree<T>::Node* child) :
    m_position(position),
    m_children {child},
           m_parent(parent)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(Node* parent, const std::vector<RapidlyExploringRandomTree<T>::Node*>& children) :
    m_position(0.0f, 0.0f),
    m_children(children),
    m_parent(parent)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(Node* parent, std::vector<RapidlyExploringRandomTree<T>::Node*>&& children) :
    m_position(0.0f, 0.0f),
    m_children(std::move(children)),
    m_parent(parent)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(const Vec2<T>& position, Node* parent, const std::vector<RapidlyExploringRandomTree<T>::Node*>& children) :
    m_position(position),
    m_children(children),
    m_parent(parent)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(const Vec2<T>& position, Node* parent, std::vector<RapidlyExploringRandomTree<T>::Node*>&& children) :
    m_position(position),
    m_children(std::move(children)),
    m_parent(parent)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(const RapidlyExploringRandomTree<T>::Node& other) :
    m_position(other.m_position),
    m_children(other.m_children)
{

}
template <class T>
RapidlyExploringRandomTree<T>::Node::Node(RapidlyExploringRandomTree<T>::Node&& other) :
    m_position(std::move(other.m_position)),
    m_children(std::move(other.m_children))
{

}

template <class T>
void RapidlyExploringRandomTree<T>::Node::addChild(RapidlyExploringRandomTree<T>::Node* newChild)
{
    m_children.push_back(newChild);
}

template <class T>
void RapidlyExploringRandomTree<T>::Node::removeChild(RapidlyExploringRandomTree<T>::Node* child)
{
    m_children.erase(std::remove(std::begin(m_children), std::end(m_children), child), std::end(m_children));
}

template <class T>
void RapidlyExploringRandomTree<T>::Node::setParent(RapidlyExploringRandomTree<T>::Node* newParent)
{
    m_parent = newParent;
}
template <class T>
const Vec2<T>& RapidlyExploringRandomTree<T>::Node::position() const
{
    return m_position;
}
template <class T>
const std::vector<typename RapidlyExploringRandomTree<T>::Node*>& RapidlyExploringRandomTree<T>::Node::children() const
{
    return m_children;
}




template <class T>
RapidlyExploringRandomTree<T>::RapidlyExploringRandomTree(const Shape2<T>& space) :
    m_space(space.clone()),
    m_randomEngine(std::make_unique<Random::Xorshift64Engine>()),
    m_randomPointPicker(*m_space)
{
    Vec2<T> center(space.center());
    m_nodes.push_back(new RapidlyExploringRandomTree<T>::Node(center, nullptr));
}

template <class T>
RapidlyExploringRandomTree<T>::RapidlyExploringRandomTree(const Shape2<T>& space, const Random::RandomEngineBase& randomEngine) :
    m_space(space.clone()),
    m_randomEngine(randomEngine.clone()),
    m_randomPointPicker(*m_space)
{
    Vec2<T> center(space.center());
    m_nodes.push_back(new RapidlyExploringRandomTree<T>::Node(center));
}

template <class T>
const Shape2<T>& RapidlyExploringRandomTree<T>::space() const
{
    return m_space;
}

template <class T>
template <class SomeShape>
void RapidlyExploringRandomTree<T>::addObstacle(const SomeShape& someShape)
{
    m_obstacles.push_back(someShape.clone().release());
}

template <class T>
void RapidlyExploringRandomTree<T>::generateNodes(size_t quantity)
{
    m_nodes.reserve(quantity + m_nodes.size());
    while(quantity--)
    {
        Vec2<T> sample;
        Edge closestEdge;
        do
        {
            sample = generateRandomSample();
            closestEdge = nearestEdge(sample);
        }
        while(!isWayClear(sample, closestEdge.targetVertex));

        Node* parentOfNewNode = nullptr;

        if(closestEdge.isNewNodeRequired)
        {
            Node* newNodeOnEdge = new Node(closestEdge.targetVertex, closestEdge.beginNode, closestEdge.endNode);
            closestEdge.beginNode->removeChild(closestEdge.endNode);
            closestEdge.beginNode->addChild(newNodeOnEdge);
            closestEdge.endNode->setParent(newNodeOnEdge);
            parentOfNewNode = newNodeOnEdge;
            addNode(newNodeOnEdge);
        }
        else
        {
            parentOfNewNode = closestEdge.closestEndNode;
        }

        Node* newNode = new Node(sample, parentOfNewNode); //closest node as parent
        parentOfNewNode->addChild(newNode);
        addNode(newNode);
    }
}
template <class T>
void RapidlyExploringRandomTree<T>::addNode(RapidlyExploringRandomTree<T>::Node* newNode)
{
    m_nodes.push_back(newNode);
}
template <class T>
const typename RapidlyExploringRandomTree<T>::Node* RapidlyExploringRandomTree<T>::nearestNode(const Vec2<T>& position) const
{
    const Node* closestNode = nullptr;
    T minDistance = std::numeric_limits<T>::max();
    if(!m_nodes.empty()) closestNode = m_nodes.back();
    for(const auto& node : m_nodes)
    {
        T distance = position.distanceTo(node->position());
        if(distance < minDistance)
        {
            minDistance = distance;
            closestNode = node;
        }
    }
    return closestNode;
}
template <class T>
typename RapidlyExploringRandomTree<T>::Node* RapidlyExploringRandomTree<T>::nearestNode(const Vec2<T>& position)
{
    Node* closestNode = nullptr;
    T minDistance = std::numeric_limits<T>::max();
    if(!m_nodes.empty()) closestNode = m_nodes.back();
    for(auto& node : m_nodes)
    {
        T distance = position.distanceTo(node->position());
        if(distance < minDistance)
        {
            minDistance = distance;
            closestNode = node;
        }
    }
    return closestNode;
}
template <class T>
typename RapidlyExploringRandomTree<T>::Edge RapidlyExploringRandomTree<T>::nearestEdge(const Vec2<T>& sample)
{
    Edge closestEdge {nullptr, nullptr, nullptr, Vec2<T>(), false};
    T closestEdgeDistance = std::numeric_limits<T>::max();
    for(auto& beginNode : m_nodes)
    {
        Vec2<T> beginNodePosition = beginNode->position();
        for(auto& endNode : beginNode->children())
        {
            Vec2<T> endNodePosition = endNode->position();

            Vec2<T> edge = endNodePosition - beginNodePosition;
            const T q = edge.quadrance();  // i.e. |w-v|^2 -  avoid a sqrt
            // Consider the line extending the segment, parameterized as v + t (w - v).
            // We find projection of point p onto the line.
            // It falls where t = [(p-v) . (w-v)] / |w-v|^2
            const T t = (sample - beginNodePosition).dot(edge) / q;

            Vec2<T> vertexOnEdge;
            Node* closestNode = nullptr;
            if(t <= 0.0)
            {
                vertexOnEdge = beginNodePosition;        // Beyond the 'v' end of the segment
                closestNode = beginNode;
            }
            else if(t >= 1.0)
            {
                vertexOnEdge = endNodePosition;   // Beyond the 'w' end of the segment
                closestNode = endNode;
            }
            else vertexOnEdge = beginNodePosition + edge * t;  // Projection falls on the segment
            T distance = sample.distanceTo(vertexOnEdge);

            if(distance < closestEdgeDistance)
            {
                closestEdge = Edge {beginNode, endNode, closestNode, vertexOnEdge, ((t > 0.0) && (t < 1.0))};
                closestEdgeDistance = distance;
            }
        }
    }
    if(closestEdge.beginNode == nullptr)
    {
        Node* onlyNode = m_nodes.back();
        Vec2<T> onlyNodePosition = onlyNode->position();
        closestEdge = Edge {onlyNode, onlyNode, onlyNode, onlyNodePosition, false};
    }
    return closestEdge;
}
template <class T>
Vec2<T> RapidlyExploringRandomTree<T>::generateRandomSample() const
{
    return m_randomPointPicker.pickRandomPoint(*m_randomEngine);
}

template <class T>
bool RapidlyExploringRandomTree<T>::isWayClear(const Vec2<T>& begin, const Vec2<T>& end) const
{
    LineSegment<T> edge(begin, end);
    for(const auto& obstacle : m_obstacles)
    {
        if(obstacle->intersects(edge))
        {
            return false;
        }
    }
    return true;
}

template <class T>
std::vector<LineSegment<T>> RapidlyExploringRandomTree<T>::edges() const
{
    std::vector<LineSegment<T>> allEdges;
    allEdges.reserve(m_nodes.size() - 1);

    for(const auto& node : m_nodes)
    {
        Vec2<T> position = node->position();
        for(const auto& child : node->children())
        {
            allEdges.emplace_back(position, child->position());
        }
    }
    return allEdges;
}
template <class T>
RapidlyExploringRandomTree<T>::~RapidlyExploringRandomTree()
{
    for(auto& obstacle : m_obstacles)
    {
        delete obstacle;
    }
    for(auto& node : m_nodes)
    {
        delete node;
    }
}

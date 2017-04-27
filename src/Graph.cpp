#include "..\include\Graph.h"

#include <unordered_map>


//TODO: after specifying requirements for custom container make sure algorithms stay along with them
// also correct algorithms where required
namespace ls
{
    template <class T, int N>
    EuclideanGraphNode<T, N>::EuclideanGraphNode(const VectorType& position) :
        m_position(position)
    {

    }

    template <class T, int N>
    auto EuclideanGraphNode<T, N>::position() const
        -> const VectorType&
    {
        return m_position;
    }
    namespace
    {
		template <class NodeHandle>
        GraphEdge<NodeHandle>::GraphEdge(NodeHandle _u, NodeHandle _v) :
            u(_u),
            v(_v)
        {

        }
		template <class NodeHandle>
        GraphEdge<NodeHandle>::GraphEdge(NodeHandle _u, const GraphHalfEdge<NodeHandle>& halfEdge) :
            u(_u),
            v(halfEdge.v)
        {

        }
		template <class NodeHandle>
        GraphHalfEdge<NodeHandle>::GraphHalfEdge(NodeHandle _v) :
            v(_v)
        {

        }
    }
    template <class NodeHandle, class T>
    WeightedGraphEdge<NodeHandle, T>::WeightedGraphEdge(NodeHandle _u, NodeHandle _v, T w) :
        GraphEdge(_u, _v),
        m_weight(w)
    {

    }
    template <class NodeHandle, class T>
    WeightedGraphEdge<NodeHandle, T>::WeightedGraphEdge(NodeHandle _u, const WeightedGraphHalfEdge<NodeHandle, T>& halfEdge) :
        GraphEdge(_u, halfEdge.v),
        m_weight(halfEdge.weight())
    {

    }
    template <class NodeHandle, class T>
    T WeightedGraphEdge<NodeHandle, T>::weight() const
    {
        return m_weight;
    }

    template <class NodeHandle, class T>
    WeightedGraphHalfEdge<NodeHandle, T>::WeightedGraphHalfEdge(NodeHandle _v, T w) :
        GraphHalfEdge(_v),
        m_weight(w)
    {

    }
    template <class NodeHandle, class T>
    T WeightedGraphHalfEdge<NodeHandle, T>::weight() const
    {
        return m_weight;
    }

    template <bool IsDirected, class StorageTypeReference>
    template <class... Args>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::emplaceNode(Args&&... args)
		-> NodeHandle
    {
        m_nodes.emplace_back(std::forward<Args>(args)...);
        m_edgesToNeighbours.emplace_back(NeighbourEdgeSetStorageType{});
        return m_nodes.size() - 1;
    }
    template <bool IsDirected, class StorageTypeReference>
    template <class... Args>
    void Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::emplaceEdge(NodeHandle u, NodeHandle v, Args&&... args)
    {
        m_edges.emplace_back(u, v, std::forward<Args>(args)...);
        const int newEdgePos = m_edges.size() - 1;

        m_edgesToNeighbours[u].emplace(newEdgePos);
        if (!isDirected) m_edgesToNeighbours[v].emplace(newEdgePos);
    }

    template <bool IsDirected, class StorageTypeReference>
    void Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::removeNode(NodeHandle v)
    {
        using std::remove_if;
        using std::begin;
        using std::end;

        const int numberOfEdges = m_edges.size();
        const int numberOfNodes = m_nodes.size();

        std::vector<int> edgeOffsetsAfterRemoval(numberOfEdges, 0);
        int currentOffset = 0;

        m_nodes.erase(std::next(begin(m_nodes), v));
        m_edgesToNeighbours.erase(std::next(begin(m_edgesToNeighbours), v));

        //remember for each edge how many places to the left it will shift after removal
        for (int i = 0; i < numberOfEdges; ++i)
        {
            edgeOffsetsAfterRemoval[i] = currentOffset;

            const EdgeType& edge = m_edges[i];
            if (edge.u == v || edge.v == v)
            {
                currentOffset += 1;
            }
        }

        //erase edges to neighbours that connect to or from v
        for (auto& neighbourSet : m_edgesToNeighbours)
        {
            for (auto iter = begin(neighbourSet); iter != end(neighbourSet); ) {
                const auto& edgeIndex = *iter;
                const auto& edge = m_edges[edgeIndex];
                if (edge.u == v || edge.v == v) {
                    iter = neighbourSet.erase(iter);
                }
                else {
                    ++iter;
                }
            }
        }

        //offset edges to neighbours
        for (auto& neighbourSet : m_edgesToNeighbours)
        {
            for (const int& edge : neighbourSet)
            {
                int& mutableEdge = const_cast<int&>(edge); //we won't alter the order of values
                mutableEdge -= edgeOffsetsAfterRemoval[edge];
            }
        }

        //actually remove edges
        m_edges.erase(
            remove_if(
                begin(m_edges),
                end(m_edges),
                [v](const EdgeType& edge)->bool {
            return (edge.u == v || edge.v == v);
        }
                ),
            end(m_edges)
            );

        //update edges to point to right node
        for (auto& edge : m_edges)
        {
            if (edge.u > v) edge.u -= 1;
            if (edge.v > v) edge.v -= 1;
        }

    }
    template <bool IsDirected, class StorageTypeReference>
    void Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::removeEdge(NodeHandle u, NodeHandle v)
    {
        using std::find_if;
        using std::remove;
        using std::begin;
        using std::end;

        auto edgeIter = find_if(
            begin(m_edges),
            end(m_edges),
            [u, v](const EdgeType& edge)->bool {
            return (edge.u == u && edge.v == v || (!isDirected && edge.u == v && edge.v == u));
        }
        );
        if (edgeIter == end(m_edges)) return;

        const int edgeId = edgeIter - begin(m_edges);
        //actually remove the edge
        m_edges.erase(edgeIter);

        auto& neighbourSetU = m_edgesToNeighbours[u];
        neighbourSetU.erase(neighbourSetU.find(edgeId));

        if (!isDirected)
        {
            auto& neighbourSetV = m_edgesToNeighbours[v];
            neighbourSetV.erase(neighbourSetV.find(edgeId));
        }

        //update edge indices in neighbour sets
        for (auto& neighbourSet : m_edgesToNeighbours)
        {
            for (const int& neighbour : neighbourSet)
            {
                int& mutableNeighbour = const_cast<int&>(neighbour); //we know that we don't change order
                if (neighbour > edgeId) mutableNeighbour -= 1;
            }
        }
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::nodes() const &
        -> const NodeArrayStorageType&
    {
        return m_nodes;
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::nodes() &&
        ->NodeArrayStorageType
    {
        return std::move(m_nodes);
    }
    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::edges() const &
        -> const EdgeArrayStorageType&
    {
        return m_edges;
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::edges() &&
        ->EdgeArrayStorageType
    {
        return std::move(m_edges);
    }
    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::node(NodeHandle v) const
        -> const NodeType&
    {
        return m_nodes[v];
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::edge(NodeHandle u, NodeHandle v) const
        -> const EdgeType&
    {
        using std::find_if;
        using std::begin;
        using std::end;

        const auto& connections = m_edgesToNeighbours[u];
        int pos = *(find_if(begin(connections), end(connections), [v, this](int e) {return this->m_edges[e].v == v || (!isDirected && this->m_edges[e].u == v); }));
        return m_edges[pos];
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::edgesToNeighbours(NodeHandle v) const
        -> const NeighbourEdgeSetStorageType&
    {
        return m_edgesToNeighbours[v];
    }
        template <bool IsDirected, class StorageTypeReference>
    bool Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::existsEdge(NodeHandle u, NodeHandle v) const
    {
        using std::find;
        using std::begin;
        using std::end;

        const auto& connections = m_edgesToNeighbours[u];
        auto pos = find_if(begin(connections), end(connections), [v, this](int e) {return this->m_edges[e].v == v || (!isDirected && this->m_edges[e].u == v); });
        return pos != end(connections);
    }
    template <bool IsDirected, class StorageTypeReference>
    int Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::numberOfNodes() const
    {
        return m_nodes.size();
    }

    template <bool IsDirected, class StorageTypeReference>
    int Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::nodeDegree(NodeHandle v) const
    {
        using std::count;

        if (isDirected)
        {
            return nodeIndegree<IsDirected, void>(v) + nodeOutdegree<IsDirected, void>(v); //to bypass sfinae parameter until constexpr if is supported
        }
        else
        {
            return m_edgesToNeighbours[v].size();
        }
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::nodeIndegree(NodeHandle v) const
    {
        using std::count_if;

        int indegree = 0;
        for (const auto& neighbourList : m_edgesToNeighbours)
        {
            indegree += count_if(begin(neighbourList), end(neighbourList), [v, this](int e) {return this->m_edges[e].v == v; });
        }
        return indegree;
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::nodeOutdegree(NodeHandle v) const
    {
        return m_edgesToNeighbours[v].size();
    }

    template <bool IsDirected, class StorageTypeReference>
    template <bool isEuclidean_, class Ret>
    Ret Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::edgeVector(NodeHandle u, NodeHandle v) const
    {
        return m_nodes[v].position() - m_nodes[u].position();
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::edgeWeight(NodeHandle u, NodeHandle v) const
        -> ValueType
    {
        auto distance = detail::distance(m_nodes[u], m_nodes[v]);
        if (!isWeighted)
        {
            return distance;
        }

        return distance * detail::weight(edge(u, v));
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>::edgeWeight(const EdgeType& edge) const
        -> ValueType
    {
        return detail::distance(m_nodes[edge.u], m_nodes[edge.v]) * detail::weight(edge);
    }


    template <bool IsDirected, class StorageTypeReference>
    template <class... Args>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::emplaceNode(Args&&... args)
		-> NodeHandle
    {
        m_nodes.emplace_back(std::forward<Args>(args)...);
        m_neighbours.emplace_back(NeighbourSetStorageType{});
        return m_nodes.size() - 1;
    }
    template <bool IsDirected, class StorageTypeReference>
    template <class... Args>
    void Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::emplaceEdge(NodeHandle u, NodeHandle v, Args&&... args)
    {
        m_neighbours[u].emplace(v, std::forward<Args>(args)...);
        if (!isDirected) m_neighbours[v].emplace(u, std::forward<Args>(args)...);
    }

    template <bool IsDirected, class StorageTypeReference>
    void Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::removeNode(NodeHandle v)
    {
        using std::remove;
        using std::begin;
        using std::end;

        const int last = m_nodes.size() - 1;

        m_nodes[v] = std::move(m_nodes.back());
        m_neighbours[v] = std::move(m_neighbours.back());
        m_nodes.pop_back();
        m_neighbours.pop_back();

        for (auto& neighbourList : m_neighbours)
        {
            auto vIter = neighbourList.find(v);
            auto lastIter = neighbourList.find(last);

            if (lastIter != end(neighbourList) && vIter == end(neighbourList)) //only last
            {
                neighbourList.emplace(v);
            }

            if (lastIter == end(neighbourList) && vIter != end(neighbourList)) //only v
            {
                neighbourList.erase(vIter);
            }

            if (lastIter != end(neighbourList)) //always remove last
            {
                neighbourList.erase(lastIter);
            }
        }
    }
    template <bool IsDirected, class StorageTypeReference>
    void Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::removeEdge(NodeHandle u, NodeHandle v)
    {
        using std::remove;
        using std::begin;
        using std::end;

        auto& neighbourListU = m_neighbours[u];
        neighbourListU.erase(neighbourListU.find(v));
        if (!isDirected)
        {
            auto& neighbourListV = m_neighbours[v];
            neighbourListV.erase(neighbourListV.find(u));
        }
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::nodes() const &
        -> const NodeArrayStorageType&
    {
        return m_nodes;
    }
    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::nodes() &&
        ->NodeArrayStorageType
    {
        return std::move(m_nodes);
    }
    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::node(NodeHandle v) const
        -> const NodeType&
    {
        return m_nodes[v];
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::edge(NodeHandle u, NodeHandle v) const
        -> EdgeType
    {
        using std::find;
        using std::begin;
        using std::end;

        const auto& connections = m_neighbours[u];
        const auto& halfEdge = *(connections.find(v));
        return EdgeType(u, halfEdge);
    }
    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::neighbours(NodeHandle v) const
        -> const NeighbourSetStorageType&
    {
        return m_neighbours[v];
    }
        template <bool IsDirected, class StorageTypeReference>
    bool Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::existsEdge(NodeHandle u, NodeHandle v) const
    {
        using std::find;
        using std::begin;
        using std::end;

        const auto& connections = m_neighbours[u];
        return connections.find(v) != end(connections);
    }
    template <bool IsDirected, class StorageTypeReference>
    int Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::numberOfNodes() const
    {
        return m_nodes.size();
    }

    template <bool IsDirected, class StorageTypeReference>
    int Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::nodeDegree(NodeHandle v) const
    {
        using std::count;

        if (isDirected)
        {
            //to bypass sfinae parameter until constexpr if is supported
            return nodeIndegree<IsDirected, void>(v) + nodeOutdegree<IsDirected, void>(v);
        }
        else
        {
            return m_neighbours[v].size();
        }
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::nodeIndegree(NodeHandle v) const
    {
        using std::count;

        int indegree = 0;
        for (const auto& neighbourList : m_neighbours)
        {
            indegree += count(begin(neighbourList), end(neighbourList), v);
        }
        return indegree;
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::nodeOutdegree(NodeHandle v) const
    {
        return m_neighbours[v].size();
    }

    template <bool IsDirected, class StorageTypeReference>
    template <bool isEuclidean_, class Ret>
    Ret Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::edgeVector(NodeHandle u, NodeHandle v) const
    {
        return m_nodes[v].position() - m_nodes[u].position();
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::edgeWeight(NodeHandle u, NodeHandle v) const
        -> ValueType
    {
        auto distance = detail::distance(m_nodes[u], m_nodes[v]);
        if (!isWeighted)
        {
            return distance;
        }

        return distance * detail::weight(edge(u, v));
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<NeighbourSetTag, IsDirected, StorageTypeReference>::edgeWeight(const EdgeType& edge) const
        -> ValueType
    {
        return detail::distance(m_nodes[edge.u], m_nodes[edge.v]) * detail::weight(edge);
    }



    template <bool IsDirected, class StorageTypeReference>
    template <class... Args>
    auto Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::emplaceNode(Args&&... args)
		-> NodeHandle
    {
        m_nodes.emplace_back(std::forward<Args>(args)...);
        if (m_nodes.size() > m_adjacencyMatrix.width())
        {
            int newSize = m_adjacencyMatrix.width() * 2;
            if (newSize == 0) newSize += 1;
            resizeAdjacencyMatrix(newSize);
        }

        return m_nodes.size() - 1;
    }
    template <bool IsDirected, class StorageTypeReference>
    void Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::emplaceEdge(NodeHandle u, NodeHandle v)
    {
        m_adjacencyMatrix(u, v) = true;
        if (!isDirected) m_adjacencyMatrix(v, u) = true;
    }

    template <bool IsDirected, class StorageTypeReference>
    void Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::removeNode(NodeHandle v)
    {
        const int newNumberOfNodes = m_nodes.size() - 1;
        const int lastElement = newNumberOfNodes;
        m_nodes[v] = std::move(m_nodes.back());
        m_nodes.pop_back();

        m_adjacencyMatrix(lastElement, v) = false;
        m_adjacencyMatrix(v, lastElement) = false;

        for (int x = 0; x < newNumberOfNodes; ++x)
        {
            m_adjacencyMatrix(x, v) = m_adjacencyMatrix(x, lastElement);
            m_adjacencyMatrix(x, lastElement) = false;
        }
        for (int y = 0; y < newNumberOfNodes; ++y)
        {
            m_adjacencyMatrix(v, y) = m_adjacencyMatrix(lastElement, y);
            m_adjacencyMatrix(lastElement, y) = false;
        }

        m_adjacencyMatrix(v, v) = m_adjacencyMatrix(lastElement, lastElement);
        m_adjacencyMatrix(lastElement, lastElement) = false;
    }
    template <bool IsDirected, class StorageTypeReference>
    void Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::removeEdge(NodeHandle u, NodeHandle v)
    {
        m_adjacencyMatrix(u, v) = false;
        if (!isDirected) m_adjacencyMatrix(v, u) = false;
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::nodes() const &
        -> const NodeArrayStorageType&
    {
        return m_nodes;
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::nodes() &&
        ->NodeArrayStorageType
    {
        return std::move(m_nodes);
    }
    template <bool IsDirected, class StorageTypeReference>
    auto Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::node(NodeHandle v) const
        -> const NodeType&
    {
        return m_nodes[v];
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::neighbours(NodeHandle v) const
        -> NeighbourSetStorageType
    {
        NeighbourSetStorageType result;

        const int numberOfNodes = m_nodes.size();
        for (int y = 0; y < numberOfNodes; ++y)
        {
            if (m_adjacencyMatrix(v, y) || (!isDirected && m_adjacencyMatrix(y, v))) result.emplace(y);
        }

        return result;
    }
    template <bool IsDirected, class StorageTypeReference>
    bool Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::existsEdge(NodeHandle u, NodeHandle v) const
    {
        return m_adjacencyMatrix(u, v) || (!isDirected && m_adjacencyMatrix(v, u));
    }
    template <bool IsDirected, class StorageTypeReference>
    int Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::numberOfNodes() const
    {
        return m_nodes.size();
    }

    template <bool IsDirected, class StorageTypeReference>
    int Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::nodeDegree(NodeHandle v) const
    {
        if (isDirected)
        {
            return nodeIndegree<IsDirected, void>(v) + nodeOutdegree<IsDirected, void>(v); //to bypass sfinae parameter until constexpr if is supported
        }
        else
        {
            const int numberOfNodes = m_nodes.size();

            int degree = 0;
            for (int u = 0; u < numberOfNodes; ++u)
            {
                degree += m_adjacencyMatrix(v, u);
            }
            degree += m_adjacencyMatrix(v, v); //count self cycle twice

            return degree;
        }
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::nodeIndegree(NodeHandle v) const
    {
        const int numberOfNodes = m_nodes.size();

        int degree = 0;
        for (int u = 0; u < numberOfNodes; ++u)
        {
            degree += m_adjacencyMatrix(u, v);
        }
        return degree;
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::nodeOutdegree(NodeHandle v) const
    {
        const int numberOfNodes = m_nodes.size();

        int degree = 0;
        for (int u = 0; u < numberOfNodes; ++u)
        {
            degree += m_adjacencyMatrix(v, u);
        }
        return degree;
    }

    template <bool IsDirected, class StorageTypeReference>
    template <bool isEuclidean_, class Ret>
    Ret Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::edgeVector(NodeHandle u, NodeHandle v) const
    {
        return m_nodes[v].position() - m_nodes[u].position();
    }
    template <bool IsDirected, class StorageTypeReference>
    void Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>::resizeAdjacencyMatrix(int newSize)
    {
        const int oldSize = m_adjacencyMatrix.width();

        Array2<bool> newAdjacencyMatrix(newSize, newSize, false);
        for (int x = 0; x < oldSize; ++x)
        {
            for (int y = 0; y < oldSize; ++y)
            {
                newAdjacencyMatrix(x, y) = m_adjacencyMatrix(x, y);
            }
        }
        m_adjacencyMatrix = std::move(newAdjacencyMatrix);
    }



    template <bool IsDirected, class StorageTypeReference>
    template <class... Args>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::emplaceNode(Args&&... args)
		-> NodeHandle
    {
        m_nodes.emplace_back(std::forward<Args>(args)...);
        return m_nodes.size() - 1;
    }
    template <bool IsDirected, class StorageTypeReference>
    template <class... Args>
    void Graph<EdgeSetTag, IsDirected, StorageTypeReference>::emplaceEdge(NodeHandle u, NodeHandle v, Args&&... args)
    {
        m_edges.emplace(u, v, std::forward<Args>(args)...);
    }

    template <bool IsDirected, class StorageTypeReference>
    void Graph<EdgeSetTag, IsDirected, StorageTypeReference>::removeNode(NodeHandle v)
    {
        using std::begin;
        using std::end;

        m_nodes.erase(std::next(begin(m_nodes), v));

        for (auto iter = begin(m_edges); iter != end(m_edges); ) {
            const auto& edge = *iter;
            if (edge.u == v || edge.v == v) {
                iter = m_edges.erase(iter);
            }
            else {
                ++iter;
            }
        }

        for (const EdgeType& edge : m_edges)
        {
            EdgeType& mutableEdge = const_cast<EdgeType&>(edge); //we know that we don't change order
            if (mutableEdge.u > v) mutableEdge.u -= 1;
            if (mutableEdge.v > v) mutableEdge.v -= 1;
        }
        m_nodes.pop_back();
    }
    template <bool IsDirected, class StorageTypeReference>
    void Graph<EdgeSetTag, IsDirected, StorageTypeReference>::removeEdge(NodeHandle u, NodeHandle v)
    {
        using std::begin;
        using std::end;

        auto uvIter = m_edges.find(GraphEdge<NodeHandle>(u, v));
        if (uvIter != end(m_edges))
        {
            m_edges.erase(uvIter);
        }
        else if (!isDirected) //only one instance of each edge is in the set, so if it was deleter already then vu won't be in the set
        {
            auto vuIter = m_edges.find(GraphEdge<NodeHandle>(v, u));
            if (vuIter != end(m_edges))
            {
                m_edges.erase(vuIter);
            }
        }
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::nodes() const &
        -> const NodeArrayStorageType&
    {
        return m_nodes;
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::nodes() &&
        ->NodeArrayStorageType
    {
        return std::move(m_nodes);
    }
    template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::node(NodeHandle v) const
        -> const NodeType&
    {
        return m_nodes[v];
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::edge(NodeHandle u, NodeHandle v) const
        -> const EdgeType&
    {
        using std::find;
        using std::begin;
        using std::end;

        auto uvIter = m_edges.find(GraphEdge<NodeHandle>(u, v));
        if (uvIter != end(m_edges))
        {
            return *uvIter;
        }
        else if (!isDirected) //only one instance of each edge is in the set, so if it was deleted already then vu won't be in the set
        {
            auto vuIter = m_edges.find(GraphEdge<NodeHandle>(v, u));
            if (vuIter != end(m_edges))
            {
                return *vuIter;
            }
        }

        //TODO: do proper exceptions or something
        throw std::runtime_error("TODO");
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::edges() const &
        -> const EdgeSetStorageType&
    {
        return m_edges;
    }
        template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::edges() &&
        ->EdgeSetStorageType
    {
        return std::move(m_edges);
    }
    template <bool IsDirected, class StorageTypeReference>
    bool Graph<EdgeSetTag, IsDirected, StorageTypeReference>::existsEdge(NodeHandle u, NodeHandle v) const
    {
        using std::find_if;
        using std::begin;
        using std::end;

        auto uvIter = m_edges.find(GraphEdge<NodeHandle>(u, v));
        if (uvIter != end(m_edges))
        {
            return true;
        }
        else if (!isDirected) //only one instance of each edge is in the set, so if it was deleted already then vu won't be in the set
        {
            auto vuIter = m_edges.find(GraphEdge<NodeHandle>(v, u));
            if (vuIter != end(m_edges))
            {
                return true;
            }
        }

        return false;
    }
    template <bool IsDirected, class StorageTypeReference>
    int Graph<EdgeSetTag, IsDirected, StorageTypeReference>::numberOfNodes() const
    {
        return m_nodes.size();
    }

    template <bool IsDirected, class StorageTypeReference>
    int Graph<EdgeSetTag, IsDirected, StorageTypeReference>::nodeDegree(NodeHandle v) const
    {
        using std::count_if;

        return count_if(
            begin(m_edges),
            end(m_edges),
            [v](const EdgeType& edge) {
            return (edge.u == v) || (edge.v == v && !isDirected);
        }
        );
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<EdgeSetTag, IsDirected, StorageTypeReference>::nodeIndegree(NodeHandle v) const
    {
        return count_if(
            begin(m_edges),
            end(m_edges),
            [v](const EdgeType& edge) {
            return (edge.v == v);
        }
        );
    }
    template <bool IsDirected, class StorageTypeReference>
    template <bool D, class SFINAE>
    int Graph<EdgeSetTag, IsDirected, StorageTypeReference>::nodeOutdegree(NodeHandle v) const
    {
        return count_if(
            begin(m_edges),
            end(m_edges),
            [v](const EdgeType& edge) {
            return (edge.u == v);
        }
        );
    }

    template <bool IsDirected, class StorageTypeReference>
    template <bool isEuclidean_, class Ret>
    Ret Graph<EdgeSetTag, IsDirected, StorageTypeReference>::edgeVector(NodeHandle u, NodeHandle v) const
    {
        return m_nodes[v].position() - m_nodes[u].position();
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::edgeWeight(NodeHandle u, NodeHandle v) const
        -> ValueType
    {
        auto distance = detail::distance(m_nodes[u], m_nodes[v]);
        if (!isWeighted)
        {
            return distance;
        }

        return distance * detail::weight(edge(u, v));
    }

    template <bool IsDirected, class StorageTypeReference>
    auto Graph<EdgeSetTag, IsDirected, StorageTypeReference>::edgeWeight(const EdgeType& edge) const
        -> ValueType
    {
        return detail::distance(m_nodes[edge.u], m_nodes[edge.v]) * detail::weight(edge);
    }
}
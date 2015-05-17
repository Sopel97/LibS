#ifndef RAPIDLYEXPLORINGRANDOMTREE_H
#define RAPIDLYEXPLORINGRANDOMTREE_H

template <class T, template <class TT = T> class NodeType> //NodeType* must be compatibile with TreeNode
class RapidlyExploringRandomTree
{
public:

    RapidlyExploringRandomTree(const Shape2<T>& space);
    RapidlyExploringRandomTree(const Shape2<T>& space, const Random::RandomEngineBase& randomEngine);

    ~RapidlyExploringRandomTree();

    template <class SomeShape>
    void addObstacle(const SomeShape& someShape);

    void generateNodes(size_t quantity);

    void addNode(NodeType<T>* newNode);

    const NodeType<T>* nearestNode(const Vec2<T>& position) const;
    NodeType<T>* nearestNode(const Vec2<T>& position);

    Vec2<T> generateRandomSample() const;

    bool isWayClear(const Vec2<T>& begin, const Vec2<T>& end) const;
    bool isWayInsideShape(const Vec2<T>& begin, const Vec2<T>& end) const;

    std::vector<LineSegment<T>> edges() const;

    const Shape2<T>& space() const;

protected:
    struct Edge
    {
        NodeType<T>* beginNode;
        NodeType<T>* endNode;
        NodeType<T>* closestEndNode;
        Vec2<T> targetVertex;
        bool isNewNodeRequired;
    };

    std::unique_ptr<Shape2<T>> m_space;
    std::unique_ptr<Random::RandomEngineBase> m_randomEngine;
    std::vector<Shape2<T>*> m_obstacles; //owning
    std::vector<NodeType<T>*> m_nodes; //owning
    RandomShapePicker<T> m_randomPointPicker;

    Edge nearestEdge(const Vec2<T>& sample);
};

typedef RapidlyExploringRandomTree<double, TreeNode> RapidlyExploringRandomTreeD;
typedef RapidlyExploringRandomTree<float, TreeNode> RapidlyExploringRandomTreeF;
typedef RapidlyExploringRandomTree<int, TreeNode> RapidlyExploringRandomTreeI;

#include "../src/RapidlyExploringRandomTree.cpp"

#endif // RAPIDLYEXPLORINGRANDOMTREE_H

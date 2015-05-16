#ifndef RAPIDLYEXPLORINGRANDOMTREE_H
#define RAPIDLYEXPLORINGRANDOMTREE_H

template <class T>
class RapidlyExploringRandomTree //TODO: make a method generating uniform random points in every Shape2 shape and allow this class to have space of arbitrary Shape2 shape
{
public:
    class Node
    {
    public:
        Node() = default;
        Node(const Vec2<T>& position, Node* parent);
        Node(Node* parent, Node* child);
        Node(const Vec2<T>& position, Node* parent, Node* child);
        Node(Node* parent, const std::vector<Node*>& children);
        Node(Node* parent, std::vector<Node*>&& children);
        Node(const Vec2<T>& position, Node* parent, const std::vector<Node*>& children);
        Node(const Vec2<T>& position, Node* parent, std::vector<Node*>&& children);

        Node(const Node& other);
        Node(Node&& other);

        void addChild(Node* newChild);
        void removeChild(Node* child);

        void setParent(Node* newParent);

        const Vec2<T>& position() const;
        const std::vector<Node*>& children() const;
    protected:
        Vec2<T> m_position;
        std::vector<Node*> m_children; //non owning
        Node* m_parent;
    };
    RapidlyExploringRandomTree(const Shape2<T>& space);
    RapidlyExploringRandomTree(const Shape2<T>& space, const Random::RandomEngineBase& randomEngine);

    ~RapidlyExploringRandomTree();

    template <class SomeShape>
    void addObstacle(const SomeShape& someShape);

    void generateNodes(size_t quantity);

    void addNode(Node* newNode);

    const Node* nearestNode(const Vec2<T>& position) const;
    Node* nearestNode(const Vec2<T>& position);

    Vec2<T> generateRandomSample() const;

    bool isWayClear(const Vec2<T>& begin, const Vec2<T>& end) const;
    bool isWayInsideShape(const Vec2<T>& begin, const Vec2<T>& end) const;

    std::vector<LineSegment<T>> edges() const;

    const Shape2<T>& space() const;

protected:
    struct Edge
    {
        Node* beginNode;
        Node* endNode;
        Node* closestEndNode;
        Vec2<T> targetVertex;
        bool isNewNodeRequired;
    };

    std::unique_ptr<Shape2<T>> m_space;
    std::unique_ptr<Random::RandomEngineBase> m_randomEngine;
    std::vector<Shape2<T>*> m_obstacles; //owning
    std::vector<Node*> m_nodes; //owning
    RandomShapePicker<T> m_randomPointPicker;

    Edge nearestEdge(const Vec2<T>& sample);
};

typedef RapidlyExploringRandomTree<double> RapidlyExploringRandomTreeD;
typedef RapidlyExploringRandomTree<float> RapidlyExploringRandomTreeF;
typedef RapidlyExploringRandomTree<int> RapidlyExploringRandomTreeI;

#include "../src/RapidlyExploringRandomTree.cpp"

#endif // RAPIDLYEXPLORINGRANDOMTREE_H

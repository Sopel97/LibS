#pragma once

#include "../Fwd.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "../Util.h"
#include "GraphRepresentationTags.h"

#include <vector>
#include <set>

//TODO consider making methods that return iterators, because now finding edge does the same as checking for existence of edge.
// to check for existence and then request edge would be waste of resources
namespace ls
{
	namespace detail
	{
		template <class T>
		struct HasMethod_position
		{
		private:
			using yes = std::true_type;
			using no = std::false_type;

			template<typename U> static auto test(int) -> decltype(std::declval<U>().position(), yes());

			template<typename> static no test(...);

		public:

			static constexpr bool value = std::is_same<decltype(test<T>(0)), yes>::value;
		};

		template <class T>
		struct HasMethod_weight
		{
		private:
			using yes = std::true_type;
			using no = std::false_type;

			template<typename U> static auto test(int) -> decltype(std::declval<U>().weight(), yes());

			template<typename> static no test(...);

		public:

			static constexpr bool value = std::is_same<decltype(test<T>(0)), yes>::value;
		};

		template <class NodeT, bool hasMethodPosition = HasMethod_position<NodeT>::value>
		auto distance(const NodeT& v1, const NodeT& v2)
			-> typename std::enable_if<hasMethodPosition, decltype(v2.position().distance(v1.position()))>::type
		{
			return v2.position().distance(v1.position());
		}

		template <class NodeT, bool hasMethodPosition = HasMethod_position<NodeT>::value>
		auto distance(const NodeT&, const NodeT&)
			-> typename std::enable_if<!hasMethodPosition, int>::type
		{
			return 1;
		}

		template <class EdgeT, bool hasMethodWeight = HasMethod_weight<EdgeT>::value>
		auto weight(const EdgeT& edge)
			-> typename std::enable_if<hasMethodWeight, decltype(edge.weight())>::type
		{
			return edge.weight();
		}

		template <class EdgeT, bool hasMethodWeight = HasMethod_weight<EdgeT>::value>
		auto weight(const EdgeT&)
			-> typename std::enable_if<!hasMethodWeight, int>::type
		{
			return 1;
		}
	}

	namespace
	{
		class GraphNode
		{

		};

		template <class NodeHandle>
		class GraphHalfEdge
		{
		public:
			NodeHandle v;

			GraphHalfEdge(NodeHandle _v);
		};

		template <class NodeHandle>
		bool operator==(const GraphHalfEdge<NodeHandle>& lhs, NodeHandle rhs)
		{
			return lhs.v == rhs;
		}
		template <class NodeHandle>
		bool operator==(NodeHandle lhs, const GraphHalfEdge<NodeHandle>& rhs)
		{
			return lhs == rhs.v;
		}
		template <class NodeHandle>
		bool operator<(const GraphHalfEdge<NodeHandle>& lhs, NodeHandle rhs)
		{
			return lhs.v < rhs;
		}
		template <class NodeHandle>
		bool operator<(NodeHandle lhs, const GraphHalfEdge<NodeHandle>& rhs)
		{
			return lhs < rhs.v;
		}

		template <class NodeHandle>
		bool operator==(const GraphHalfEdge<NodeHandle>& lhs, const GraphHalfEdge<NodeHandle>& rhs)
		{
			return lhs.v == rhs.v;
		}
		template <class NodeHandle>
		bool operator<(const GraphHalfEdge<NodeHandle>& lhs, const GraphHalfEdge<NodeHandle>& rhs)
		{
			return lhs.v < rhs.v;
		}

		template <class NodeHandle>
		class GraphEdge
		{
		public:
			NodeHandle u, v;

			GraphEdge(NodeHandle _u, NodeHandle _v);
			GraphEdge(NodeHandle _u, const GraphHalfEdge<NodeHandle>& halfEdge);
		};

		template <class NodeHandle>
		bool operator==(const GraphEdge<NodeHandle>& lhs, const GraphEdge<NodeHandle>& rhs)
		{
			return std::tie(lhs.u, lhs.v) == std::tie(rhs.u, rhs.v);
		}
		template <class NodeHandle>
		bool operator<(const GraphEdge<NodeHandle>& lhs, const GraphEdge<NodeHandle>& rhs)
		{
			return std::tie(lhs.u, lhs.v) < std::tie(rhs.u, rhs.v);
		}
	}


	template <class T, int Dim>
	class EuclideanGraphNode : public GraphNode
	{
	public:
		using ValueType = T;
		using VectorType = typename Util::VectorType<T, Dim>::type;
		constexpr static int dimensionality = Dim;

	private:
		VectorType m_position;
	public:
		EuclideanGraphNode(const VectorType& position);

		const VectorType& position() const;
	};

	using EuclideanGraphNode2D = EuclideanGraphNode<double, 2>;
	using EuclideanGraphNode2F = EuclideanGraphNode<float, 2>;

	extern template class EuclideanGraphNode<double, 2>;
	extern template class EuclideanGraphNode<float, 2>;

	template <class NodeHandle, class T>
	class WeightedGraphHalfEdge : public GraphHalfEdge<NodeHandle>
	{
	private:
		T m_weight;
	public:

		WeightedGraphHalfEdge(NodeHandle _v, T w = T(1));

		T weight() const;
	};

	template <class NodeHandle, class T>
	class WeightedGraphEdge : public GraphEdge<NodeHandle>
	{
	private:
		T m_weight;
	public:

		WeightedGraphEdge(NodeHandle _u, NodeHandle _v, T w = T(1));
		WeightedGraphEdge(NodeHandle _u, const WeightedGraphHalfEdge<NodeHandle, T>& halfEdge);

		T weight() const;
	};


	//TODO: enforce and specify the following better
	template <class T, int Dim>
	class DefaultEuclideanGraphStorageTypeReference
	{
	public:

		using ValueType = T;

		using NodeType = EuclideanGraphNode<T, Dim>;

		template <class NodeHandle>
		using EdgeType = GraphEdge<NodeHandle>;
		template <class NodeHandle>
		using HalfEdgeType = GraphHalfEdge<NodeHandle>;

		/*
		std::vector behaviour
		*/
		template <class T>
		using NodeArrayStorageType = std::vector<T>;

		/*
			std::set behaviour
		*/
		template <class T>
		using NeighbourSetStorageType = std::set<T>;


		/*
			Requires that following are defined for Container:
			- Container(width, height, value)
			- operator(x, y)
			- width()

		*/
		template <class T>
		using AdjacencyMatrixStorageType = Array2<T>;


		/*
			std::set behaviour
		*/
		template <class T>
		using EdgeSetStorageType = std::set<T>;

		/*
			std::vector behaviour
		*/
		template <class T>
		using EdgeArrayStorageType = std::vector<T>;
	};

	//NO duplicate edges allowed
	template <class GraphRepresentationTag, bool IsDirected, class StorageTypeReference>
	class Graph
	{
	public:
		Graph() = delete;
	};

	template <bool IsDirected, class StorageTypeReference>
	class Graph<NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>
	{
	public:
		using NodeHandle = int;

		using ValueType = typename StorageTypeReference::ValueType;
		using NodeType = typename StorageTypeReference::template NodeType;
		using EdgeType = typename StorageTypeReference::template EdgeType<NodeHandle>;
		using HalfEdgeType = typename StorageTypeReference::template HalfEdgeType<NodeHandle>;
		using VectorType = typename NodeType::VectorType;
		using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
		using NeighbourEdgeSetStorageType = typename StorageTypeReference::template NeighbourSetStorageType<int>;
		using NeighbourEdgeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NeighbourEdgeSetStorageType>;
		using EdgeArrayStorageType = typename StorageTypeReference::template EdgeArrayStorageType<EdgeType>;
		using Representation = NeighbourEdgeSetWithEdgeListTag;
		static constexpr bool isDirected = IsDirected;
		static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
		static constexpr bool isWeighted = detail::HasMethod_weight<EdgeType>::value && detail::HasMethod_weight<HalfEdgeType>::value;

		static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");
		static_assert(std::is_base_of<GraphEdge<NodeHandle>, EdgeType>::value, "EdgeType must be derived from GraphEdge");
		static_assert(std::is_base_of<GraphHalfEdge<NodeHandle>, HalfEdgeType>::value, "HalfEdgeType must be derived from GraphHalfEdge");

		Graph() = default;

		template <class... Args>
		NodeHandle emplaceNode(Args&&... args);
		template <class... Args>
		void emplaceEdge(NodeHandle u, NodeHandle v, Args&&... args);

		void removeNode(NodeHandle v);
		void removeEdge(NodeHandle u, NodeHandle v);

		const NodeArrayStorageType& nodes() const &;
		NodeArrayStorageType nodes() && ;
		const EdgeArrayStorageType& edges() const &;
		EdgeArrayStorageType edges() && ;

		const NodeType& node(NodeHandle v) const;
		const EdgeType& edge(NodeHandle u, NodeHandle v) const;
		const NeighbourEdgeSetStorageType& edgesToNeighbours(NodeHandle v) const;
		bool existsEdge(NodeHandle u, NodeHandle v) const;
		int numberOfNodes() const;

		int nodeDegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeIndegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeOutdegree(NodeHandle v) const;

		template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
		Ret edgeVector(NodeHandle u, NodeHandle v) const;

		ValueType edgeWeight(NodeHandle u, NodeHandle v) const;
		ValueType edgeWeight(const EdgeType& edge) const;

	protected:
		NodeArrayStorageType m_nodes;
		NeighbourEdgeArrayStorageType m_edgesToNeighbours; //holds indices of edges, NOT neighbour nodes
		EdgeArrayStorageType m_edges;
	};

	template <bool IsDirected, class StorageTypeReference>
	class Graph<NeighbourSetTag, IsDirected, StorageTypeReference>
	{
	public:
		using NodeHandle = int;

		using ValueType = typename StorageTypeReference::ValueType;
		using NodeType = typename StorageTypeReference::template NodeType;
		using EdgeType = typename StorageTypeReference::template EdgeType<NodeHandle>;
		using HalfEdgeType = typename StorageTypeReference::template HalfEdgeType<NodeHandle>;
		using VectorType = typename NodeType::VectorType;
		using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
		using NeighbourSetStorageType = typename StorageTypeReference::template NeighbourSetStorageType<HalfEdgeType>;
		using NeighbourArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NeighbourSetStorageType>;
		using Representation = NeighbourSetTag;
		static constexpr bool isDirected = IsDirected;
		static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
		static constexpr bool isWeighted = detail::HasMethod_weight<HalfEdgeType>::value;

		static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");
		static_assert(std::is_base_of<GraphEdge<NodeHandle>, EdgeType>::value, "EdgeType must be derived from GraphEdge");
		static_assert(std::is_base_of<GraphHalfEdge<NodeHandle>, HalfEdgeType>::value, "HalfEdgeType must be derived from GraphHalfEdge");

		Graph() = default;

		template <class... Args>
		int emplaceNode(Args&&... args);
		template <class... Args>
		void emplaceEdge(NodeHandle u, NodeHandle v, Args&&... args);

		void removeNode(NodeHandle v);
		void removeEdge(NodeHandle u, NodeHandle v);

		const NodeArrayStorageType& nodes() const &;
		NodeArrayStorageType nodes() && ;
		const NodeType& node(NodeHandle v) const;
		EdgeType edge(NodeHandle u, NodeHandle v) const;
		const NeighbourSetStorageType& neighbours(NodeHandle v) const;
		bool existsEdge(NodeHandle u, NodeHandle v) const;
		int numberOfNodes() const;

		int nodeDegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeIndegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeOutdegree(NodeHandle v) const;

		template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
		Ret edgeVector(NodeHandle u, NodeHandle v) const;

		ValueType edgeWeight(NodeHandle u, NodeHandle v) const;
		ValueType edgeWeight(const EdgeType& edge) const;

	protected:
		NodeArrayStorageType m_nodes;
		NeighbourArrayStorageType m_neighbours;
	};

	template <bool IsDirected, class StorageTypeReference>
	class Graph<AdjacencyMatrixTag, IsDirected, StorageTypeReference>
	{
	public:
		using NodeHandle = int;

		using ValueType = typename StorageTypeReference::ValueType;
		using NodeType = typename StorageTypeReference::template NodeType;
		using VectorType = typename NodeType::VectorType;
		using Representation = AdjacencyMatrixTag;
		using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
		using AdjacencyMatrixStorageType = typename StorageTypeReference::template AdjacencyMatrixStorageType<bool>;
		using NeighbourSetStorageType = typename StorageTypeReference::template NeighbourSetStorageType<int>;
		static constexpr bool isDirected = IsDirected;
		static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
		static constexpr bool isWeighted = false;

		static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");

		Graph() = default;

		template <class... Args>
		int emplaceNode(Args&&... args);
		void emplaceEdge(NodeHandle u, NodeHandle v);

		void removeNode(NodeHandle u);
		void removeEdge(NodeHandle u, NodeHandle v);

		const NodeArrayStorageType& nodes() const &;
		NodeArrayStorageType nodes() && ;
		const NodeType& node(NodeHandle v) const;
		NeighbourSetStorageType neighbours(NodeHandle v) const;
		bool existsEdge(NodeHandle u, NodeHandle v) const;
		int numberOfNodes() const;

		int nodeDegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeIndegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeOutdegree(NodeHandle v) const;

		template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
		Ret edgeVector(NodeHandle u, NodeHandle v) const;

	protected:
		NodeArrayStorageType m_nodes;
		AdjacencyMatrixStorageType m_adjacencyMatrix; //[x][y] is set to true if edge x,y exists

		void resizeAdjacencyMatrix(int newSize);

	};

	template <bool IsDirected, class StorageTypeReference>
	class Graph<EdgeSetTag, IsDirected, StorageTypeReference>
	{
	public:
		using NodeHandle = int;

		using ValueType = typename StorageTypeReference::ValueType;
		using NodeType = typename StorageTypeReference::template NodeType;
		using EdgeType = typename StorageTypeReference::template EdgeType<NodeHandle>;
		using VectorType = typename NodeType::VectorType;
		using Representation = EdgeSetTag;
		using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
		using EdgeSetStorageType = typename StorageTypeReference::template EdgeSetStorageType<EdgeType>;
		static constexpr bool isDirected = IsDirected;
		static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
		static constexpr bool isWeighted = detail::HasMethod_weight<EdgeType>::value;

		static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");
		static_assert(std::is_base_of<GraphEdge<NodeHandle>, EdgeType>::value, "EdgeType must be derived from GraphEdge");

		Graph() = default;

		template <class... Args>
		int emplaceNode(Args&&... args);
		template <class... Args>
		void emplaceEdge(NodeHandle u, NodeHandle v, Args&&... args);

		void removeNode(NodeHandle v);
		void removeEdge(NodeHandle u, NodeHandle v);

		const NodeArrayStorageType& nodes() const &;
		NodeArrayStorageType nodes() && ;
		const NodeType& node(NodeHandle v) const;
		const EdgeType& edge(NodeHandle u, NodeHandle v) const;
		const EdgeSetStorageType& edges() const &;
		EdgeSetStorageType edges() && ;
		bool existsEdge(NodeHandle u, NodeHandle v) const;
		int numberOfNodes() const;

		int nodeDegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeIndegree(NodeHandle v) const;
		template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
		int nodeOutdegree(NodeHandle v) const;

		template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
		Ret edgeVector(NodeHandle u, NodeHandle v) const;

		ValueType edgeWeight(NodeHandle u, NodeHandle v) const;
		ValueType edgeWeight(const EdgeType& edge) const;

	protected:
		NodeArrayStorageType m_nodes;
		EdgeSetStorageType m_edges;
	};


	using AdjacencyListEuclideanDigraph2D = Graph<NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	using AdjacencyListEuclideanDiGraph2F = Graph<NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	using AdjacencyMatrixEuclideanDiGraph2D = Graph<AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	using AdjacencyMatrixEuclideanDiGraph2F = Graph<AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	using EdgeListEuclideanDiGraph2D = Graph<EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	using EdgeListEuclideanDiGraph2F = Graph<EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	using AdjacencyListEuclideanGraph2D = Graph<NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	using AdjacencyListEuclideanGraph2F = Graph<NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	using AdjacencyMatrixEuclideanGraph2D = Graph<AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	using AdjacencyMatrixEuclideanGraph2F = Graph<AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	using EdgeListEuclideanGraph2D = Graph<EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	using EdgeListEuclideanGraph2F = Graph<EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<float, 2>>;



	extern template class Graph<NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	extern template class Graph<AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	extern template class Graph<EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	extern template class Graph<NeighbourEdgeSetWithEdgeListTag, true, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<NeighbourEdgeSetWithEdgeListTag, true, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	extern template class Graph<NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	extern template class Graph<AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	extern template class Graph<EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<float, 2>>;

	extern template class Graph<NeighbourEdgeSetWithEdgeListTag, false, DefaultEuclideanGraphStorageTypeReference<double, 2>>;
	extern template class Graph<NeighbourEdgeSetWithEdgeListTag, false, DefaultEuclideanGraphStorageTypeReference<float, 2>>;
}

#include "../src/Graph.cpp"

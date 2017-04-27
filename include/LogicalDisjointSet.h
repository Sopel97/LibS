#pragma once

#include <vector>
#include <numeric>

namespace ls
{
	class LogicalDisjointSet
	{
	public:
		using SetHandle = int;
	private:
		struct Node
		{
			SetHandle parent;
			int rank;
		};
		std::vector<Node> m_nodes;
	public:
		LogicalDisjointSet(size_t n) :
			m_nodes(n)
		{
			int i = 0;
			for (auto& node : m_nodes)
			{
				node.parent = i;
				node.rank = 0;
				++i;
			}
		}

		// s1, s2 must be primary sets
		SetHandle setUnion(SetHandle s1, SetHandle s2)
		{
			if (s1 == s2) return s1;
			if (m_nodes[s1].rank > m_nodes[s2].rank) std::swap(s1, s2);

			if (m_nodes[s1].rank == m_nodes[s2].rank) ++m_nodes[s2].rank;
			m_nodes[s1].parent = s2;

			return s2;
		}

		SetHandle rootOf(SetHandle s)
		{
			while (!isRoot(s))
			{
				m_nodes[s].parent = rootOf(m_nodes[s].parent);
			}

			return m_nodes[s].parent;
		}

		bool isRoot(SetHandle s)
		{
			return m_nodes[s].parent == s;
		}
	};
}
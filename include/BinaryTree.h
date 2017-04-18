#pragma once

#include <memory>

namespace ls
{
	template <class T>
	class BinaryTree
	{
	private:
		struct Node
		{
			friend class BinaryTree<T>;
		public:

			~Node() = default;

			Node& left()
			{
				return *m_left;
			}
			Node& right()
			{
				return *m_right;
			}
			Node& parent()
			{
				return *m_parent;
			}
			const Node& left() const
			{
				return *m_left;
			}
			const Node& right() const
			{
				return *m_right;
			}
			const Node& parent() const
			{
				return *m_parent;
			}
			bool hasLeft()
			{
				return m_left != nullptr;
			}
			bool hasRight()
			{
				return m_right != nullptr;
			}
			bool hasParent()
			{
				return m_parent != nullptr;
			}
			bool isLeaf()
			{
				return !(hasLeft() || hasRight());
			}
			T& value()
			{
				return m_value;
			}
			const T& value() const
			{
				return m_value;
			}
		protected:
			template <class U>
			Node(U&& v, std::unique_ptr<Node>&& l, std::unique_ptr<Node>&& r, Node* p) :
				m_value(std::forward<U>(v)),
				m_left(std::move(l)),
				m_right(std::move(r)),
				m_parent(p)
			{

			}
			template <class... Args>
			Node(std::unique_ptr<Node>&& l, std::unique_ptr<Node>&& r, Node* p, Args&&... args) :
				m_value(std::forward<Args>(args)...),
				m_left(std::move(l)),
				m_right(std::move(r)),
				m_parent(p)
			{

			}

			Node(Node&& other) = default;
			Node& operator=(Node&& other) = default;

			T m_value;
			std::unique_ptr<Node> m_left;
			std::unique_ptr<Node> m_right;
			Node* m_parent;
		};

	public:
		BinaryTree(const T& root) :
			m_root(new Node(root, nullptr, nullptr, nullptr))
		{

		}
		BinaryTree(T&& root) :
			m_root(new Node(std::move(root), nullptr, nullptr, nullptr))
		{

		}

		BinaryTree(const BinaryTree& other) :
			m_root(deepCopy(other.m_root))
		{
		}
		BinaryTree(BinaryTree&& other) :
			m_root(std::move(other.m_root))
		{

		}

		BinaryTree<T>& operator=(const BinaryTree& other)
		{
			m_root = deepCopy(other.m_root);

			return *this;
		}
		BinaryTree<T>& operator=(BinaryTree&& other)
		{
			m_root = std::move(other.m_root);
			return *this;
		}

		~BinaryTree()
		{

		}


		Node& insertLeft(Node& node, const T& newElement)
		{
			return insert(node, newElement, &Node::m_left);
		}
		Node& insertLeft(Node& node, T&& newElement)
		{
			return insert(node, std::move(newElement), &Node::m_left);
		}
		Node& insertRight(Node& node, const T& newElement)
		{
			return insert(node, newElement, &Node::m_right);
		}
		Node& insertRight(Node& node, T&& newElement)
		{
			return insert(node, std::move(newElement), &Node::m_right);
		}
		template <class... Args>
		Node& emplaceLeft(Node& node, Args&&... args)
		{
			return emplace(node, &Node::m_left, std::forward<Args>(args)...);
		}
		template <class... Args>
		Node& emplaceRight(Node& node, Args&&... args)
		{
			return emplace(node, &Node::m_right, std::forward<Args>(args)...);
		}

		Node& root()
		{
			return *m_root;
		}
		const Node& root() const
		{
			return *m_root;
		}

	private:
		std::unique_ptr<Node> m_root;

		template <class U>
		Node& insert(Node& node, U&& newElement, std::unique_ptr<Node> Node::*child)
		{
			// new is required because Node has a private constructor
			std::unique_ptr<Node> newNode(new Node(std::forward<U>(newElement), nullptr, nullptr, &node));
			(newNode.get())->*child = std::move(node.*child);
			node.*child = std::move(newNode);

			return *(node.*child);
		}

		template <class... Args>
		Node& emplace(Node& node, std::unique_ptr<Node> Node::*child, Args&&... args)
		{
			// new is required because Node has a private constructor
			std::unique_ptr<Node> newNode(new Node(nullptr, nullptr, &node, std::forward<Args>(args)...));
			(newNode.get())->*child = std::move(node.*child);
			node.*child = std::move(newNode);

			return *(node.*child);
		}

		std::unique_ptr<Node> deepCopy(const std::unique_ptr<Node>& node)
		{
			std::unique_ptr<Node> l = nullptr;
			if (node->m_left != nullptr) l = deepCopy(node->m_left);

			std::unique_ptr<Node> r = nullptr;
			if (node->m_right != nullptr) r = deepCopy(node->m_right);

			std::unique_ptr<Node> nodeCopy(new Node(node->m_value, std::move(l), std::move(r), nullptr));
			if (nodeCopy->m_left != nullptr) nodeCopy->m_left->m_parent = nodeCopy.get();
			if (nodeCopy->m_right != nullptr) nodeCopy->m_right->m_parent = nodeCopy.get();

			return nodeCopy;
		}
	};
}
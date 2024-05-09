#pragma once

#include "../util/types.hpp"

#include <memory>
#include <vector>

namespace reni::rg {

	class RenderNode;
	class NodeVisitor;



	/**
	 * @brief Owning pointer to a RenderNode.
	*/
	template <std::derived_from<RenderNode> Node>
	using NodePtr = std::shared_ptr<Node>;



	/**
	 * @brief List of RenderNode%s.
	*/
	using NodeList = std::vector<NodePtr<RenderNode>>; 



	/**
	 * @brief Defines a single RenderGraph item.
	*/
	class RenderNode : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderNode() = default;

		virtual void accept(NodeVisitor& visitor) const = 0;


		/**
		 * @brief List of all child nodes.
		*/
		const NodeList& children() const {
			return m_children;
		}


		/**
		 * @brief Add a child node to the node.
		*/
		void addChild(NodePtr<RenderNode> n) {
			m_children.push_back(n);
		}


	private:
		NodeList m_children;
	};



	/**
	 * @brief Create a RenderNode of the specified type.
	*/
	template <std::derived_from<RenderNode> Node, typename... Args>
	NodePtr<Node> makeNode(Args&&... args) {
		return std::make_shared<Node>(std::forward<Args>(args)...);
	}

}
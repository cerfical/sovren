#ifndef RENI_RG_RENDER_NODE_HEADER
#define RENI_RG_RENDER_NODE_HEADER

#include <memory>
#include <vector>

namespace RENI::rg {
	class RenderNode;
	class NodeVisitor;



	/**
	 * @brief Pointer to a RenderGraph node.
	*/
	template <std::derived_from<RenderNode> Node>
	using NodePtr = std::shared_ptr<Node>;



	/**
	 * @brief List of RenderGraph nodes.
	*/
	template <std::derived_from<RenderNode> Node>
	using NodeList = std::vector<NodePtr<Node>>;



	/**
	 * @brief Defines a single RenderGraph item.
	*/
	class RenderNode {
	public:

		RenderNode() = default;

		RenderNode(const RenderNode&) = delete;
		RenderNode& operator=(const RenderNode&) = delete;

		virtual ~RenderNode() = default;



		/** @{ */
		/**
		 * @brief Perform an operation that depends on both the node and the NodeVisitor.
		*/
		virtual void acceptVisitor(NodeVisitor& visitor) const = 0;



		/**
		 * @brief List of all child nodes.
		*/
		const NodeList<RenderNode>& children() const {
			return m_children;
		}



		/**
		 * @brief Add a child node to the node.
		*/
		void addChild(NodePtr<RenderNode> n) {
			m_children.push_back(n);
		}
		/** @} */



	private:
		NodeList<RenderNode> m_children;
	};



	/**
	 * @brief Create a RenderGraph node of the specified type.
	*/
	template <std::derived_from<RenderNode> Node, typename... Args>
	NodePtr<Node> makeNode(Args&&... args) {
		return std::make_shared<Node>(std::forward<Args>(args)...);
	}

}

#endif
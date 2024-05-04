#ifndef RENI_RENDER_GRAPH_HEADER
#define RENI_RENDER_GRAPH_HEADER

#include "rg/RenderNode.hpp"
#include "utils.hpp"

namespace reni {

	/**
	 * @brief Describes the geometry and visual properties of a graphics scene to be rendered.
	*/
	class RenderGraph : private NonCopyable {
	public:


		/**
		 * @brief Add a root node to the graph.
		*/
		void addNode(rg::NodePtr<rg::RenderNode> node) {
			m_rootNode.addChild(node);
		}


		/**
		 * @brief List of root nodes of the graph.
		*/
		std::span<const rg::NodePtr<rg::RenderNode>> nodes() const {
			return m_rootNode.children();
		}


	private:
		rg::RenderNode m_rootNode;
	};

}

#endif
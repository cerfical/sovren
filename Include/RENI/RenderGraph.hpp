#ifndef RENI_RENDER_GRAPH_HEADER
#define RENI_RENDER_GRAPH_HEADER

#include "rg/RenderNode.hpp"

namespace RENI {

	/**
	 * @brief Describes the geometry and visual properties of a graphics scene to be rendered.
	*/
	class RenderGraph {
	public:

		RenderGraph() = default;

		RenderGraph(const RenderGraph&) = delete;
		RenderGraph& operator=(const RenderGraph&) = delete;

		RenderGraph(RenderGraph&&) = default;
		RenderGraph& operator=(RenderGraph&&) = default;



		/**
		 * @brief Add a root node to the graph.
		*/
		void addRootNode(rg::NodePtr<rg::RenderNode> node) {
			m_rootNodes.push_back(node);
		}



		/**
		 * @brief List of root nodes of the graph.
		*/
		const rg::NodeList<rg::RenderNode>& rootNodes() const {
			return m_rootNodes;
		}



	private:
		rg::NodeList<rg::RenderNode> m_rootNodes;
	};

}

#endif
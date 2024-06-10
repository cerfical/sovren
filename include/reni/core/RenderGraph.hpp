#pragma once

#include "../rg/RenderNode.hpp"

#include "../util/NonCopyable.hpp"

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
		const rg::NodeList& nodes() const {
			return m_rootNode.children();
		}


	private:
		class RootNode : public rg::RenderNode {
		public:
			void accept(rg::NodeVisitor&) const override {}
		};

		RootNode m_rootNode;
	};

}
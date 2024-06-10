#pragma once

#include "../rg/SceneNode.hpp"

#include <utility>

namespace reni {

    /**
     * @brief Describes the geometry and visual properties of a graphics scene to be rendered.
     */
    class RenderGraph {
    public:

        RenderGraph(const RenderGraph&) = delete;
        RenderGraph& operator=(const RenderGraph&) = delete;

        RenderGraph(RenderGraph&&) = delete;
        RenderGraph& operator=(RenderGraph&&) = delete;

        RenderGraph() = default;
        ~RenderGraph() = default;


        /**
         * @brief Add a root node to the graph.
         */
        void addNode(SceneNodePtr<SceneNode> node) {
            rootNode_.addChild(std::move(node));
        }


        /**
         * @brief List of root nodes of the graph.
         */
        const SceneNodeList& nodes() const {
            return rootNode_.children();
        }


    private:
        class RootNode : public SceneNode {
        public:
            void accept(NodeVisitor&) const override {}
        };

        RootNode rootNode_;
    };

}

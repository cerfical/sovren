#pragma once

#include "sg/SceneNode.hpp"

#include <utility>

namespace sovren {

    /**
     * @brief Describes the geometry and visual properties of a graphics scene to be rendered.
     */
    class SceneGraph {
    public:

        SceneGraph() = default;

        SceneGraph(const SceneGraph&) = delete;
        auto operator=(const SceneGraph&) -> SceneGraph& = delete;

        SceneGraph(SceneGraph&&) = delete;
        auto operator=(SceneGraph&&) -> SceneGraph& = delete;

        ~SceneGraph() = default;


        /**
         * @brief Add a root node to the graph.
         */
        void addNode(SceneNodePtr<SceneNode> node) {
            rootNode_.addChild(std::move(node));
        }


        /**
         * @brief List of root nodes of the graph.
         */
        [[nodiscard]]
        auto nodes() const -> const SceneNodeList& {
            return rootNode_.children();
        }


    private:
        class RootNode : public SceneNode {
        public:
            void acceptVisitor(NodeVisitor&) const override {}
        };

        RootNode rootNode_;
    };

}

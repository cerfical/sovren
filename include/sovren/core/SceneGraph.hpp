#pragma once

#include "../sg/SceneNode.hpp"

#include <utility>

namespace sovren {

    /**
     * @brief Describes the geometry and visual properties of a graphics scene to be rendered.
     */
    class SceneGraph {
    public:

        SceneGraph(const SceneGraph&) = delete;
        SceneGraph& operator=(const SceneGraph&) = delete;

        SceneGraph(SceneGraph&&) = delete;
        SceneGraph& operator=(SceneGraph&&) = delete;

        SceneGraph() = default;
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

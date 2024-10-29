#pragma once

#include <concepts>
#include <memory>
#include <utility>
#include <vector>

namespace sovren {

    class SceneNode;
    class NodeVisitor;


    /**
     * @brief Owning pointer to a SceneNode.
     */
    template <std::derived_from<SceneNode> Node>
    using SceneNodePtr = std::shared_ptr<Node>;


    /**
     * @brief List of SceneNode%s.
     */
    using SceneNodeList = std::vector<SceneNodePtr<SceneNode>>;


    /**
     * @brief Defines a single SceneGraph item.
     */
    class SceneNode {
    public:

        SceneNode() = default;

        SceneNode(const SceneNode&) = delete;
        auto operator=(const SceneNode&) -> SceneNode& = delete;

        SceneNode(SceneNode&&) = delete;
        auto operator=(SceneNode&&) -> SceneNode& = delete;

        virtual ~SceneNode() = default;


        /**
         * @brief Perform a type-safe 'visit' on the node using a visitor.
         */
        virtual void acceptVisitor(NodeVisitor& visitor) const = 0;


        /**
         * @brief List of all child nodes.
         */
        [[nodiscard]]
        auto children() const noexcept -> const SceneNodeList& {
            return children_;
        }


        /**
         * @brief Add a child node to the node.
         */
        void addChild(SceneNodePtr<SceneNode> node) {
            children_.push_back(std::move(node));
        }


    private:
        SceneNodeList children_;
    };


    /**
     * @brief Create a SceneNode of the specified type.
     */
    template <std::derived_from<SceneNode> Node, typename... Args>
        requires std::constructible_from<Node, Args...>
    [[nodiscard]]
    auto makeSceneNode(Args&&... args) -> SceneNodePtr<Node> {
        return std::make_shared<Node>(std::forward<Args>(args)...);
    }

}

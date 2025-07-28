#pragma once

#include "core/Size2.hpp"

#include "pal/Keys.hpp"
#include "pal/MouseButtons.hpp"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

namespace sovren {

    class SceneNode;
    class Input;

    using NodePtr = std::shared_ptr<SceneNode>;
    using NodeList = std::vector<NodePtr>;

    class SceneNode : public std::enable_shared_from_this<SceneNode> {
    public:
        SceneNode() = default;

        SceneNode(const SceneNode&) = delete;
        auto operator=(const SceneNode&) -> SceneNode& = delete;

        SceneNode(SceneNode&&) = delete;
        auto operator=(SceneNode&&) -> SceneNode& = delete;

        virtual ~SceneNode() = default;

        void update() {
            onUpdate();
        }

        [[nodiscard]]
        auto input() const noexcept -> const Input*;

        void addChild(NodePtr n) {
            children_.emplace_back(n);
            // TODO: Do something with std::enable_shared_from_this?
            n->setParent(shared_from_this());
        }

        void removeChild(SceneNode* n) noexcept {
            const auto removed = std::ranges::remove(children_, n, [](const auto& n) {
                return n.get();
            });
            children_.erase(removed.begin(), removed.end());
        }

        [[nodiscard]]
        auto children() const noexcept -> const NodeList& {
            return children_;
        }

        void setParent(NodePtr n) noexcept {
            parent_ = n;
        }

        [[nodiscard]]
        auto parent() const noexcept -> NodePtr {
            return parent_.lock();
        }

    protected:
        // TODO: Provide more convenient way to handle events
        virtual void onWindowClose() {
            notifyChildren(&SceneNode::onWindowClose);
        }

        virtual void onWindowResize(Size2i size) {
            notifyChildren(&SceneNode::onWindowResize, size);
        }

        virtual void onKeyDown(Keys k) {
            notifyChildren(&SceneNode::onKeyDown, k);
        }

        virtual void onKeyUp(Keys k) {
            notifyChildren(&SceneNode::onKeyUp, k);
        }

        virtual void onMouseButtonDown(MouseButtons b) {
            notifyChildren(&SceneNode::onMouseButtonDown, b);
        }

        virtual void onMouseButtonUp(MouseButtons b) {
            notifyChildren(&SceneNode::onMouseButtonUp, b);
        }

        virtual void onMouseMove(Size2i offset) {
            notifyChildren(&SceneNode::onMouseMove, offset);
        }

        virtual void onUpdate() {
            notifyChildren(&SceneNode::onUpdate);
        }

    private:
        template <typename... Params, typename... Args>
        void notifyChildren(void (SceneNode::*event)(Params...), Args&&... args) {
            for(const auto& child : children()) {
                (child.get()->*event)(std::forward<Args>(args)...);
            }
        }

        std::weak_ptr<SceneNode> parent_;
        NodeList children_;
    };

    template <std::derived_from<SceneNode> Node, typename... Args>
        requires std::constructible_from<Node, Args...>
    [[nodiscard]]
    auto makeNode(Args&&... args) -> std::shared_ptr<Node> {
        return std::make_shared<Node>(std::forward<Args>(args)...);
    }

}

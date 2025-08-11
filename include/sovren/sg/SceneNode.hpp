#pragma once

#include <sovren/log/Log.hpp>

#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>

#include <sovren/util/Size2.hpp>

#include <sovren/Input.hpp>

#include <memory>
#include <utility>
#include <vector>

namespace sovren::sg {

    class SceneNode;

    using NodePtr = std::shared_ptr<SceneNode>;
    using NodeList = std::vector<NodePtr>;

    class SceneNode {
    public:
        SceneNode() = default;

        SceneNode(const SceneNode&) = delete;
        auto operator=(const SceneNode&) -> SceneNode& = delete;

        SceneNode(SceneNode&&) = delete;
        auto operator=(SceneNode&&) -> SceneNode& = delete;

        virtual ~SceneNode() = default;

        virtual void init() {
            callChildren(&SceneNode::init);
        }

        virtual void free() {
            callChildren(&SceneNode::free);
        }

        virtual void update() {
            callChildren(&SceneNode::update);
        }

        [[nodiscard]]
        virtual auto input() noexcept -> const Input* {
            if(auto* const n = findUp<SceneNode>()) {
                return n->input();
            }
            return nullptr;
        }

        [[nodiscard]]
        virtual auto log() noexcept -> log::Log* {
            if(auto* const n = findUp<SceneNode>()) {
                return n->log();
            }
            return nullptr;
        }

        template <std::derived_from<SceneNode> Node, typename... Args>
            requires std::constructible_from<Node, Args...>
        auto makeChild(Args&&... args) -> std::shared_ptr<Node> {
            auto n = std::make_shared<Node>(std::forward<Args>(args)...);
            addChild(n);
            return n;
        }

        void addChild(NodePtr n) {
            children_.push_back(n);
            n->parent_ = this;
        }

        void removeChild(SceneNode* n) noexcept {
            std::erase_if(children_, [n](const auto& child) {
                return child.get() == n;
            });
        }

        [[nodiscard]]
        auto children() const noexcept -> const NodeList& {
            return children_;
        }

        [[nodiscard]]
        auto parent() const noexcept -> SceneNode* {
            return parent_;
        }

        template <std::derived_from<SceneNode> Node>
        [[nodiscard]]
        auto findDown() const noexcept -> Node* {
            for(const auto& child : children()) {
                if(auto* const n = dynamic_cast<Node*>(child.get())) {
                    return n;
                }
            }
            return nullptr;
        }

        template <std::derived_from<SceneNode> Node>
        [[nodiscard]]
        auto findUp() const noexcept -> Node* {
            auto* p = parent();
            while(p) {
                if(auto* const n = dynamic_cast<Node*>(p)) {
                    return n;
                }
                p = p->parent();
            }
            return nullptr;
        }

    protected:
        // TODO: Provide more convenient way to handle events
        virtual void onWindowClose() {
            callChildren(&SceneNode::onWindowClose);
        }

        virtual void onWindowResize(Size2i size) {
            callChildren(&SceneNode::onWindowResize, size);
        }

        virtual void onKeyDown(pal::Key k) {
            callChildren(&SceneNode::onKeyDown, k);
        }

        virtual void onKeyUp(pal::Key k) {
            callChildren(&SceneNode::onKeyUp, k);
        }

        virtual void onMouseButtonDown(pal::MouseButton b) {
            callChildren(&SceneNode::onMouseButtonDown, b);
        }

        virtual void onMouseButtonUp(pal::MouseButton b) {
            callChildren(&SceneNode::onMouseButtonUp, b);
        }

        virtual void onMouseMove(Size2i offset) {
            callChildren(&SceneNode::onMouseMove, offset);
        }

    private:
        template <typename... Params, typename... Args>
        void callChildren(void (SceneNode::*f)(Params...), Args&&... args) {
            for(const auto& child : children()) {
                (child.get()->*f)(std::forward<Args>(args)...);
            }
        }

        SceneNode* parent_ = {};
        NodeList children_;
    };

    template <std::derived_from<SceneNode> Node, typename... Args>
        requires std::constructible_from<Node, Args...>
    [[nodiscard]]
    auto makeNode(Args&&... args) -> std::shared_ptr<Node> {
        return std::make_shared<Node>(std::forward<Args>(args)...);
    }

}

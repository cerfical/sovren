#pragma once

#include <sovren/log/Log.hpp>

#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>

#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <sovren/Event.hpp>
#include <sovren/Input.hpp>
#include <sovren/KeyEvent.hpp>
#include <sovren/KeyEventType.hpp>
#include <sovren/MouseEvent.hpp>
#include <sovren/MouseEventType.hpp>
#include <sovren/RenderList.hpp>
#include <sovren/RenderServer.hpp>
#include <sovren/Window.hpp>
#include <sovren/WindowEvent.hpp>
#include <sovren/WindowEventType.hpp>

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

        virtual void render(RenderList& list) {
            callChildren(&SceneNode::render, list);
        }

        [[nodiscard]]
        virtual auto input() const noexcept -> const Input* {
            if(const auto* const n = findUp<SceneNode>()) {
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

        [[nodiscard]]
        virtual auto window() noexcept -> Window* {
            if(auto* const n = findUp<SceneNode>()) {
                return n->window();
            }
            return nullptr;
        }

        [[nodiscard]]
        virtual auto server() noexcept -> RenderServer* {
            if(auto* const n = findUp<SceneNode>()) {
                return n->server();
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
        virtual void onEvent(const Event& e) {
            if(const auto* const we = e.as<WindowEvent>()) {
                onWindowEvent(*we);
            } else if(const auto* const me = e.as<MouseEvent>()) {
                onMouseEvent(*me);
            } else if(const auto* const ke = e.as<KeyEvent>()) {
                onKeyEvent(*ke);
            }

            callChildren(&SceneNode::onEvent, e);
        }

        virtual void onWindowEvent(const WindowEvent& e) {
            switch(e.type) {
                case WindowEventType::Resize: onWindowResize(e.size); break;
                case WindowEventType::Close:  onWindowClose(); break;
                case WindowEventType::None:   break;
            }

            callChildren(&SceneNode::onWindowEvent, e);
        }

        virtual void onKeyEvent(const KeyEvent& e) {
            switch(e.type) {
                case KeyEventType::KeyDown: onKeyDown(e.key); break;
                case KeyEventType::KeyUp:   onKeyUp(e.key); break;
                case KeyEventType::None:    break;
            }

            callChildren(&SceneNode::onKeyEvent, e);
        }

        virtual void onMouseEvent(const MouseEvent& e) {
            switch(e.type) {
                case MouseEventType::Move:       onMouseMove(e.pos); break;
                case MouseEventType::ButtonDown: onMouseButtonDown(e.button); break;
                case MouseEventType::ButtonUp:   onMouseButtonUp(e.button); break;
                case MouseEventType::None:       break;
            }

            callChildren(&SceneNode::onMouseEvent, e);
        }

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

        virtual void onMouseMove(Point2i pos) {
            callChildren(&SceneNode::onMouseMove, pos);
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

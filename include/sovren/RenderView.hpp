#pragma once

#include "pal/EventHandler.hpp"
#include "pal/EventPump.hpp"
#include "pal/Keys.hpp"
#include "pal/MouseButtons.hpp"
#include "pal/Window.hpp"

#include "rhi/RenderContext.hpp"
#include "rhi/RenderDevice.hpp"
#include "rhi/SwapChain.hpp"

#include "sg/Camera3D.hpp"
#include "sg/Line2D.hpp"
#include "sg/MeshNode3D.hpp"
#include "sg/NodeVisitor.hpp"
#include "sg/Rect2D.hpp"
#include "sg/SceneNode.hpp"

#include "Color.hpp"
#include "Point2.hpp"
#include "SceneGraph.hpp"
#include "Size2.hpp"

#include <algorithm>
#include <memory>
#include <stack>
#include <unordered_map>
#include <vector>

namespace sovren {

    class RenderView : private EventHandler, private NodeVisitor {
    public:

        RenderView(const RenderView&) = delete;
        auto operator=(const RenderView&) -> RenderView& = delete;

        RenderView(RenderView&&) = delete;
        auto operator=(RenderView&&) -> RenderView& = delete;

        virtual ~RenderView() = default;


        RenderView()
            : window_(Window::create()), mousePos_(window_->mousePos()) {

            renderDevice_ = RenderDevice::create();
            swapChain_ = renderDevice_->createSwapChain(window_->handle());
            renderContext_ = renderDevice_->createRenderContext();
            window_->setEventHandler(this);

            setFill(Color::white());
        }


        [[nodiscard]]
        auto keyState(Keys k) const -> bool {
            return std::ranges::find(pressedKeys_, k) != pressedKeys_.cend();
        }


        [[nodiscard]]
        auto buttonState(MouseButtons b) const -> bool {
            return std::ranges::find(pressedButtons_, b) != pressedButtons_.cend();
        }


        [[nodiscard]]
        auto mousePos() const -> Point2 {
            return mousePos_;
        }


        [[nodiscard]]
        auto window() -> Window* {
            return window_.get();
        }


        [[nodiscard]]
        auto scene() -> SceneGraph* {
            return &scene_;
        }


        [[nodiscard]]
        auto fill() const -> Color {
            return fillColor_;
        }


        void setFill(Color fill) {
            fillColor_ = fill;
        }


        [[nodiscard]]
        auto size() const -> Size2 {
            return window_->size();
        }


        [[nodiscard]]
        auto width() const -> int {
            return size().width;
        }


        [[nodiscard]]
        auto height() const -> int {
            return size().height;
        }


        void show() {
            window_->setVisible(true);
            visible_ = true;

            const auto events = EventPump::create();
            while(visible_) {
                events->pumpEvents();
                onUpdate();
            }
        }

        void hide() {
            window_->setVisible(false);
            visible_ = false;
        }


    protected:
        void onKeyDown(Keys k) override {
            if(!keyState(k)) {
                pressedKeys_.push_back(k);
            }
        }

        void onKeyUp(Keys k) override {
            if(keyState(k)) {
                std::erase(pressedKeys_, k);
            }
        }


        void onMouseButtonDown(MouseButtons b) override {
            if(!buttonState(b)) {
                pressedButtons_.push_back(b);
            }
        }

        void onMouseButtonUp(MouseButtons b) override {
            if(buttonState(b)) {
                std::erase(pressedButtons_, b);
            }
        }

        virtual void onMouseMove(int dx, int dy) {
            static_cast<void>(dx);
            static_cast<void>(dy);
        }


        virtual void onUpdate() {
            renderContext_->startDraw();
            renderContext_->setTarget(swapChain_->frontBuffer());

            renderContext_->clear(fillColor_);

            transformStack2d_.push(Mat3x3::identity());
            transformStack3d_.push(Mat4x4::identity());
            viewProjStack_.push(Mat4x4::identity());

            visitNodes(scene_.nodes());

            viewProjStack_.pop();
            transformStack3d_.pop();
            transformStack2d_.pop();

            renderContext_->endDraw();
            swapChain_->swapBuffers();
        }


    private:
        void onWindowClose() final {
            hide();
        }

        void onWindowResize() final {
            swapChain_->setBufferSize(window_->size());
        }

        void onMouseMove() final {
            const auto old = std::exchange(mousePos_, window_->mousePos());
            onMouseMove(mousePos_.x - old.x, mousePos_.y - old.y);
        }


        void visit(const Line2D& l) final {
            transformStack2d_.push(transformStack2d_.top() * l.transform().toMatrix());

            renderContext_->painter().setTransform(transformStack2d_.top());
            renderContext_->painter().drawLine(l.startPoint(), l.endPoint());
            visitNodes(l.children());

            transformStack2d_.pop();
        }

        void visit(const Rect2D& r) final {
            transformStack2d_.push(transformStack2d_.top() * r.transform().toMatrix());

            renderContext_->painter().setTransform(transformStack2d_.top());
            renderContext_->painter().drawRect(r.topLeftPoint(), r.bottomRightPoint());
            visitNodes(r.children());

            transformStack2d_.pop();
        }


        void visit(const MeshNode3D& m) final {
            transformStack3d_.push(transformStack3d_.top() * m.transform().toMatrix());

            const auto [it, ok] = meshes_.try_emplace(&m);
            if(ok) {
                it->second = renderDevice_->createVertexBuffer(std::as_bytes(std::span(m.vertices())));
            }

            renderContext_->setTransform(transformStack3d_.top() * viewProjStack_.top());
            renderContext_->drawMesh(*it->second);
            visitNodes(m.children());

            transformStack3d_.pop();
        }

        void visit(const Camera3D& c) final {
            transformStack3d_.push(transformStack3d_.top() * c.transform().toMatrix());
            viewProjStack_.push(
                invert(transformStack3d_.top()) * // calculate the camera view matrix based on position in the scene
                c.toProjMatrix()                  // then apply the projection matrix
            );

            // make all child nodes to be positioned independently of the camera
            transformStack3d_.push(Mat4x4::identity());
            visitNodes(c.children());
            transformStack3d_.pop();

            transformStack3d_.pop();
            viewProjStack_.pop();
        }


        void visitNodes(const SceneNodeList& n) {
            for(const auto& c : n) {
                c->acceptVisitor(*this);
            }
        }


        std::vector<MouseButtons> pressedButtons_;
        std::vector<Keys> pressedKeys_;
        std::unique_ptr<Window> window_;

        Point2 mousePos_;
        bool visible_ = {};


        std::unordered_map<const SceneNode*, std::unique_ptr<VertexBuffer>> meshes_;

        std::stack<Mat3x3> transformStack2d_;
        std::stack<Mat4x4> transformStack3d_;
        std::stack<Mat4x4> viewProjStack_;

        std::unique_ptr<RenderDevice> renderDevice_;
        std::unique_ptr<RenderContext> renderContext_;
        std::unique_ptr<SwapChain> swapChain_;

        Color fillColor_;
        SceneGraph scene_;
    };

}

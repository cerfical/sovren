#pragma once

#include "input/EventHandler.hpp"
#include "input/Keys.hpp"
#include "input/MouseButtons.hpp"

#include "pal/EventPump.hpp"
#include "pal/Window.hpp"

#include "rhi/Render2D.hpp"
#include "rhi/Render3D.hpp"
#include "rhi/RenderBackend.hpp"
#include "rhi/SwapChain.hpp"

#include "sg/Camera3D.hpp"
#include "sg/Line2D.hpp"
#include "sg/NodeVisitor.hpp"
#include "sg/Rect2D.hpp"
#include "sg/SceneNode.hpp"
#include "sg/Transform2D.hpp"
#include "sg/Transform3D.hpp"
#include "sg/Triangle3D.hpp"

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

            renderApi_ = RenderBackend::create();
            swapChain_ = renderApi_->createSwapChain(window_->nativeHandle());
            render2D_ = renderApi_->createRender2D();
            render3D_ = renderApi_->createRender3D();
            window_->setEventHandler(this);

            setFill(Color::fromRgb(1.0f, 1.0f, 1.0f));
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
            render3D_->startRender(swapChain_->frontBuffer());
            render2D_->startRender(swapChain_->frontBuffer());

            render3D_->clear(fillColor_);

            transformStack2d_.push(Mat3x3::identity());
            transformStack3d_.push(Mat4x4::identity());
            viewProjStack_.push(Mat4x4::identity());

            visitNodes(scene_.nodes());

            viewProjStack_.pop();
            transformStack3d_.pop();
            transformStack2d_.pop();

            render2D_->endRender();
            render3D_->endRender();
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
            render2D_->setTransform(transformStack2d_.top());
            render2D_->drawLine(l.startPoint(), l.endPoint());
            visitNodes(l.children());
        }

        void visit(const Rect2D& r) final {
            render2D_->setTransform(transformStack2d_.top());
            render2D_->drawRect(r.topLeftPoint(), r.bottomRightPoint());
            visitNodes(r.children());
        }

        void visit(const Transform2D& t) final {
            // make 2D geometry child nodes to render relative to their parent
            transformStack2d_.push(transformStack2d_.top() * t.toMatrix());
            visitNodes(t.children());
            transformStack2d_.pop();
        }


        void visit(const Triangle3D& t) final {
            const auto [it, ok] = meshes_.try_emplace(&t);
            if(ok) {
                it->second = renderApi_->createVertexBuffer(std::as_bytes(t.points()));
            }

            render3D_->setTransform(transformStack3d_.top() * viewProjStack_.top());
            render3D_->drawMesh(*it->second);
            visitNodes(t.children());
        }

        void visit(const Transform3D& t) final {
            // make 3D geometry child nodes to render relative to their parent
            transformStack3d_.push(transformStack3d_.top() * t.toMatrix());
            visitNodes(t.children());
            transformStack3d_.pop();
        }

        void visit(const Camera3D& c) final {
            viewProjStack_.push(
                invert(transformStack3d_.top()) * // calculate the camera view matrix based on position in the scene
                c.toProjMatrix()                  // then apply the projection matrix
            );

            // make all child nodes to be positioned independently of the camera
            transformStack3d_.push(Mat4x4::identity());

            visitNodes(c.children());

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

        std::unique_ptr<RenderBackend> renderApi_;
        std::unique_ptr<Render2D> render2D_;
        std::unique_ptr<Render3D> render3D_;
        std::unique_ptr<SwapChain> swapChain_;

        Color fillColor_;
        SceneGraph scene_;
    };

}

#pragma once

#include "Point2.hpp"
#include "Size2.hpp"

#include "core/Renderer.hpp"
#include "core/SceneGraph.hpp"
#include "input/EventHandler.hpp"
#include "pal/EventPump.hpp"
#include "pal/Window.hpp"

#include <algorithm>
#include <memory>
#include <vector>

namespace sovren {

    class RenderView : private EventHandler {
    public:

        RenderView(const RenderView&) = delete;
        auto operator=(const RenderView&) -> RenderView& = delete;

        RenderView(RenderView&&) = delete;
        auto operator=(RenderView&&) -> RenderView& = delete;

        virtual ~RenderView() = default;


        RenderView()
            : window_(Window::create()), mousePos_(window_->mousePos()), renderer_(*window_) {
            window_->setEventHandler(this);
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
            return renderer_.clearColor();
        }


        void setFill(Color fill) {
            renderer_.setClearColor(fill);
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
            renderer_.renderScene(scene_);
        }


    private:
        void onWindowClose() final {
            hide();
        }

        void onWindowResize() final {
            renderer_.setRenderSize(window_->size());
        }

        void onMouseMove() final {
            const auto old = std::exchange(mousePos_, window_->mousePos());
            onMouseMove(mousePos_.x - old.x, mousePos_.y - old.y);
        }

        std::vector<MouseButtons> pressedButtons_;
        std::vector<Keys> pressedKeys_;
        std::unique_ptr<Window> window_;

        Point2 mousePos_;
        bool visible_ = {};

        Renderer renderer_;
        SceneGraph scene_;
    };

}

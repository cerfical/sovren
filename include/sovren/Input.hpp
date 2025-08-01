#pragma once

#include "core/Point2.hpp"

#include "pal/EventHandler.hpp"
#include "pal/Keys.hpp"
#include "pal/MouseButtons.hpp"
#include "pal/Window.hpp"

#include "SceneNode.hpp"

#include <algorithm>

namespace sovren {

    class Input final : public EventHandler {
    public:
        explicit Input(Window* window, SceneNode* target)
            : window_(window), target_(target) {
            mousePos_ = window->mousePos();
            window->setEventHandler(this);
        }

        ~Input() {
            window_->setEventHandler(nullptr);
        }

        [[nodiscard]]
        auto isDown(Keys k) const noexcept -> bool {
            return std::ranges::find(pressedKeys_, k) != pressedKeys_.cend();
        }

        [[nodiscard]]
        auto isUp(Keys k) const noexcept -> bool {
            return !isDown(k);
        }

        [[nodiscard]]
        auto isDown(MouseButtons b) const noexcept -> bool {
            return std::ranges::find(pressedButtons_, b) != pressedButtons_.cend();
        }

        [[nodiscard]]
        auto isUp(MouseButtons b) const noexcept -> bool {
            return !isDown(b);
        }

        [[nodiscard]]
        auto mousePos() const noexcept -> Point2i {
            return mousePos_;
        }

    private:
        void onWindowClose() override {
            target_->onWindowClose();
        }

        void onWindowResize() override {
            target_->onWindowResize(window_->size());
        }

        void onKeyDown(Keys k) override {
            if(isUp(k)) {
                pressedKeys_.push_back(k);
                target_->onKeyDown(k);
            }
        }

        void onKeyUp(Keys k) override {
            if(isDown(k)) {
                std::erase(pressedKeys_, k);
                target_->onKeyUp(k);
            }
        }

        void onMouseButtonDown(MouseButtons b) override {
            if(isUp(b)) {
                pressedButtons_.push_back(b);
                target_->onMouseButtonDown(b);
            }
        }

        void onMouseButtonUp(MouseButtons b) override {
            if(isDown(b)) {
                std::erase(pressedButtons_, b);
                target_->onMouseButtonUp(b);
            }
        }

        void onMouseMove() override {
            const auto oldPos = std::exchange(mousePos_, window_->mousePos());
            const auto newPos = mousePos_;
            target_->onMouseMove({ newPos.x - oldPos.x, newPos.y - oldPos.y });
        }

        SceneNode* target_ = {};
        Window* window_ = {};

        Point2i mousePos_;
        std::vector<MouseButtons> pressedButtons_;
        std::vector<Keys> pressedKeys_;
    };

}

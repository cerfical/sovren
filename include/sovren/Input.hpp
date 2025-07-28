#pragma once

#include "core/Point2.hpp"

#include "pal/EventHandler.hpp"
#include "pal/Keys.hpp"
#include "pal/MouseButtons.hpp"
#include "pal/Window.hpp"

#include "SceneNode.hpp"

#include <algorithm>
#include <memory>

namespace sovren {

    class Input : public SceneNode {
    public:
        explicit Input(std::shared_ptr<Window> window)
            : window_(window), events_(this) {
            mousePos_ = window->mousePos();
            window->setEventHandler(&events_);
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
        class EventDispatcher final : public EventHandler {
        public:
            explicit EventDispatcher(Input* input) noexcept
                : input_(input) {
            }

            void onWindowClose() override {
                input_->onWindowClose();
            }

            void onWindowResize() override {
                input_->onWindowResize(input_->window_->size());
            }

            void onKeyDown(Keys k) override {
                if(input_->isUp(k)) {
                    input_->pressedKeys_.push_back(k);
                    input_->onKeyDown(k);
                }
            }

            void onKeyUp(Keys k) override {
                if(input_->isDown(k)) {
                    std::erase(input_->pressedKeys_, k);
                    input_->onKeyUp(k);
                }
            }

            void onMouseButtonDown(MouseButtons b) override {
                if(input_->isUp(b)) {
                    input_->pressedButtons_.push_back(b);
                    input_->onMouseButtonDown(b);
                }
            }

            void onMouseButtonUp(MouseButtons b) override {
                if(input_->isDown(b)) {
                    std::erase(input_->pressedButtons_, b);
                    input_->onMouseButtonUp(b);
                }
            }

            void onMouseMove() override {
                const auto oldPos = std::exchange(input_->mousePos_, input_->window_->mousePos());
                const auto newPos = input_->mousePos_;
                input_->onMouseMove({ newPos.x - oldPos.x, newPos.y - oldPos.y });
            }

        private:
            Input* input_ = {};
        };

        std::shared_ptr<Window> window_;
        EventDispatcher events_;

        Point2i mousePos_;
        std::vector<MouseButtons> pressedButtons_;
        std::vector<Keys> pressedKeys_;
    };

    inline auto SceneNode::input() const noexcept -> const Input* {
        if(const auto* const i = dynamic_cast<const Input*>(this)) {
            return i;
        }
        if(const auto p = parent()) {
            return p->input();
        }
        return nullptr;
    }

}

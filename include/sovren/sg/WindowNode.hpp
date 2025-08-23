#pragma once

#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>

#include <sovren/sg/SceneNode.hpp>

#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <sovren/Input.hpp>
#include <sovren/Window.hpp>

namespace sovren::sg {

    class WindowNode : public SceneNode {
    public:
        void init() override {
            SceneNode::init();
            window_.show();
        }

        void free() override {
            window_.hide();
            SceneNode::free();
        }

        void update() override {
            auto& events = window_.events();
            while(events) {
                onEvent(events.pop());
            }

            SceneNode::update();
        }

        [[nodiscard]]
        auto input() const noexcept -> const Input* override {
            return &input_;
        }

        [[nodiscard]]
        auto window() noexcept -> Window* override {
            return &window_;
        }

    protected:
        void onWindowResize(Size2i size) override {
            window_.setSize(size);
            SceneNode::onWindowResize(size);
        }

        void onWindowClose() override {
            SceneNode::onWindowClose();
            window_.hide();
        }

        void onKeyDown(pal::Key k) override {
            if(input_.isUp(k)) {
                input_.press(k);
                SceneNode::onKeyDown(k);
            }
        }

        void onKeyUp(pal::Key k) override {
            if(input_.isDown(k)) {
                input_.release(k);
                SceneNode::onKeyUp(k);
            }
        }

        void onMouseButtonDown(pal::MouseButton b) override {
            if(input_.isUp(b)) {
                input_.press(b);
                SceneNode::onMouseButtonDown(b);
            }
        }

        void onMouseButtonUp(pal::MouseButton b) override {
            if(input_.isDown(b)) {
                input_.release(b);
                SceneNode::onMouseButtonUp(b);
            }
        }

        void onMouseMove(Point2i pos) override {
            const auto oldPos = input_.mousePos();
            if(oldPos != pos) {
                input_.setMousePos(pos);
                SceneNode::onMouseMove(pos);
            }
        }

    private:
        Window window_;
        Input input_;
    };

}

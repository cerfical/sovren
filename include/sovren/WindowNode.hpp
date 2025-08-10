#pragma once

#include "pal/Window.hpp"
#include "pal/WindowHandler.hpp"
#include "rhi/SwapChain.hpp"
#include "rhi/Texture.hpp"

#include "Input.hpp"
#include "RenderNode.hpp"

#include <memory>

namespace sovren {

    class WindowNode : public RenderNode, public WindowHandler {
    public:
        void init() override {
            window_ = Window::create(this);
            swapChain_ = device()->createSwapChain(window_->handle());

            RenderNode::init();

            window_->setVisible(true);
        }

        void free() override {
            window_->setVisible(false);

            RenderNode::free();

            swapChain_.reset();
            window_.reset();
        }

        void update() override {
            RenderNode::update();
            swapChain_->swapBuffers();
        }

        [[nodiscard]]
        auto input() noexcept -> const Input* override {
            return &input_;
        }

        [[nodiscard]]
        auto target() noexcept -> rhi::Texture* override {
            return swapChain_->frontBuffer();
        }

        [[nodiscard]]
        auto window() noexcept -> Window* override {
            return window_.get();
        }

    protected:
        void onWindowResize(Size2i size) override {
            RenderNode::onWindowResize(size);
            swapChain_->resizeBuffers(size);
        }

        void onWindowClose() override {
            RenderNode::onWindowClose();
            window_->setVisible(false);
        }

    private:
        void windowClose() override {
            onWindowClose();
        }

        void windowResize() override {
            onWindowResize(window_->size());
        }

        void keyDown(Keys k) override {
            if(input_.isUp(k)) {
                input_.press(k);
                onKeyDown(k);
            }
        }

        void keyUp(Keys k) override {
            if(input_.isDown(k)) {
                input_.release(k);
                onKeyUp(k);
            }
        }

        void mouseButtonDown(MouseButtons b) override {
            if(input_.isUp(b)) {
                input_.press(b);
                onMouseButtonDown(b);
            }
        }

        void mouseButtonUp(MouseButtons b) override {
            if(input_.isDown(b)) {
                input_.release(b);
                onMouseButtonUp(b);
            }
        }

        void mouseMove() override {
            const auto oldPos = input_.mousePos();
            const auto newPos = window_->mousePos();

            if(oldPos != newPos) {
                input_.setMousePos(newPos);
                onMouseMove({ newPos.x - oldPos.x, newPos.y - oldPos.y });
            }
        }

        std::shared_ptr<Window> window_;
        std::unique_ptr<rhi::SwapChain> swapChain_;
        Input input_;
    };

}

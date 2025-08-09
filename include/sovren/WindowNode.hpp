#pragma once

#include "pal/EventPump.hpp"
#include "pal/Window.hpp"
#include "pal/WindowHandler.hpp"
#include "rhi/Device.hpp"
#include "rhi/SwapChain.hpp"
#include "rhi/Texture.hpp"

#include "Input.hpp"
#include "RenderNode.hpp"

#include <memory>

namespace sovren {

    class WindowNode : public RenderNode, public WindowHandler {
    public:
        WindowNode()
            : window_(Window::create(this))
            , events_(EventPump::create())
            , device_(rhi::Device::create()) {
            swapChain_ = device_->createSwapChain(window_->handle());
        }

        [[nodiscard]]
        auto input() noexcept -> const Input* override {
            return &input_;
        }

        [[nodiscard]]
        auto device() noexcept -> rhi::Device* override {
            return device_.get();
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
            SceneNode::onWindowResize(size);
            swapChain_->resizeBuffers(size);
        }

        void onWindowClose() override {
            SceneNode::onWindowClose();
            window_->setVisible(false);
        }

        void onUpdate() override {
            window_->setVisible(true);
            while(window_->isVisible()) {
                events_->pumpEvents();

                SceneNode::onUpdate();
                swapChain_->swapBuffers();
            }
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
        std::unique_ptr<EventPump> events_;

        std::unique_ptr<rhi::Device> device_;
        std::unique_ptr<rhi::SwapChain> swapChain_;

        Input input_;
    };

}

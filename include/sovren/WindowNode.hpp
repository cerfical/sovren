#pragma once

#include "pal/EventPump.hpp"
#include "pal/Window.hpp"

#include "rhi/Device.hpp"
#include "rhi/SwapChain.hpp"
#include "rhi/Texture.hpp"

#include "Input.hpp"
#include "RenderNode.hpp"

#include <memory>

namespace sovren {

    class WindowNode : public RenderNode {
    public:
        WindowNode()
            : window_(Window::create())
            , events_(EventPump::create())
            , input_(window_.get(), this)
            , device_(rhi::Device::create()) {
            swapChain_ = device_->createSwapChain(window_->handle());
        }

        [[nodiscard]]
        auto input() noexcept -> Input* override {
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
        std::shared_ptr<Window> window_;
        std::unique_ptr<EventPump> events_;

        std::unique_ptr<rhi::Device> device_;
        std::unique_ptr<rhi::SwapChain> swapChain_;

        Input input_;
    };

}

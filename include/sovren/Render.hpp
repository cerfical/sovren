#pragma once

#include "pal/Window.hpp"

#include "rhi/Device.hpp"
#include "rhi/SwapChain.hpp"

#include "math/Mat3x3.hpp"

#include "SceneNode.hpp"
#include "SceneNode2D.hpp"

#include <memory>
#include <stack>

namespace sovren {

    class Render : public SceneNode {
    public:
        explicit Render(std::shared_ptr<Window> window)
            : window_(window), device_(rhi::Device::create()) {
            swapChain_ = device_->createSwapChain(window->handle());
            context2D_ = device_->createContext2D();
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
            SceneNode::onUpdate();

            renderScene(*this, *swapChain_->frontBuffer());
            swapChain_->swapBuffers();
        }

    private:
        void renderScene(const SceneNode& scene, rhi::Texture& rt) {
            transforms_.push(Mat3x3::identity());

            context2D_->setRenderTarget(&rt);
            context2D_->beginDraw();

            context2D_->clear(Color::white());
            renderNode(scene);

            context2D_->endDraw();
            context2D_->setRenderTarget(nullptr);

            transforms_.pop();
        }

        void renderNode(const SceneNode& n) {
            if(const auto* const node = dynamic_cast<const SceneNode2D*>(&n)) {
                renderNode2D(*node);
            } else {
                renderChildren(n);
            }
        }

        void renderNode2D(const SceneNode2D& n) {
            transforms_.push(transforms_.top() * n.transform().matrix());

            const auto& m = transforms_.top();
            context2D_->setTransform({
                { m[0][0], m[0][1], m[1][0], m[1][1], m[2][0], m[2][1] },
            });

            n.render(*context2D_);
            renderChildren(n);

            transforms_.pop();
        }

        void renderChildren(const SceneNode& n) {
            for(const auto& child : n.children()) {
                renderNode(*child);
            }
        }

        std::shared_ptr<Window> window_;

        std::unique_ptr<rhi::Device> device_;
        std::unique_ptr<rhi::SwapChain> swapChain_;
        std::unique_ptr<rhi::Context2D> context2D_;

        std::stack<Mat3x3> transforms_;
    };

}

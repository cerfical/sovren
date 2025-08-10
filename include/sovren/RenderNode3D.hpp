#pragma once

#include "rhi/Context3D.hpp"

#include "RenderNode.hpp"
#include "Renderer3D.hpp"

namespace sovren {

    class RenderNode3D : public RenderNode {
    public:
        void init() override {
            context_ = device()->createContext3D();
            RenderNode::init();
        }

        void free() override {
            RenderNode::free();

            context_.reset();
            renderer_ = Renderer3D();
        }

        void update() override {
            RenderNode::update();
            renderer_.renderScene(*this);
        }

        auto context3D() noexcept -> rhi::Context3D* override {
            return context_.get();
        }

    private:
        std::unique_ptr<rhi::Context3D> context_;
        Renderer3D renderer_;
    };

}

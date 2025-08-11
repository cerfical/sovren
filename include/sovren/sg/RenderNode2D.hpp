#pragma once

#include <sovren/rhi/Context2D.hpp>

#include <sovren/sg/RenderNode.hpp>

#include <sovren/Renderer2D.hpp>

#include <memory>

namespace sovren::sg {

    class RenderNode2D : public RenderNode {
    public:
        void init() override {
            context_ = device()->createContext2D();
            RenderNode::init();
        }

        void free() override {
            RenderNode::free();

            context_.reset();
            renderer_ = Renderer2D();
        }

        void update() override {
            RenderNode::update();
            renderer_.renderScene(*this);
        }

        auto context2D() noexcept -> rhi::Context2D* override {
            return context_.get();
        }

    private:
        std::unique_ptr<rhi::Context2D> context_;
        Renderer2D renderer_;
    };

}

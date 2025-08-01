#pragma once

#include "rhi/Context2D.hpp"

#include "RenderNode.hpp"
#include "Renderer2D.hpp"

#include <memory>

namespace sovren {

    class RenderNode2D : public RenderNode {
    public:
        auto context2D() noexcept -> rhi::Context2D* override {
            return context_.get();
        }

    protected:
        void onUpdate() override {
            SceneNode::onUpdate();

            if(!context_) {
                context_ = device()->createContext2D();
            }
            renderer_.renderScene(*this);
        }

    private:
        std::unique_ptr<rhi::Context2D> context_;
        Renderer2D renderer_;
    };

}

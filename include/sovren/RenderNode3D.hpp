#pragma once

#include "rhi/Context3D.hpp"

#include "RenderNode.hpp"
#include "Renderer3D.hpp"

namespace sovren {

    class RenderNode3D : public RenderNode {
    public:
        auto context3D() noexcept -> rhi::Context3D* override {
            return context_.get();
        }

    protected:
        void onUpdate() override {
            SceneNode::onUpdate();

            if(!context_) {
                context_ = device()->createContext3D();
            }
            renderer_.renderScene(*this);
        }

    private:
        std::unique_ptr<rhi::Context3D> context_;
        Renderer3D renderer_;
    };

}

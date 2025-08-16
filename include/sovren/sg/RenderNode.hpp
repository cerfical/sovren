#pragma once

#include <sovren/rhi/Context2D.hpp>
#include <sovren/rhi/Context3D.hpp>
#include <sovren/rhi/Device.hpp>

#include <sovren/sg/SceneNode.hpp>

namespace sovren::sg {

    class RenderNode : public SceneNode {
    public:
        virtual void render() {}

        [[nodiscard]]
        virtual auto device() noexcept -> rhi::Device* {
            if(auto* const n = findUp<RenderNode>()) {
                return n->device();
            }
            return nullptr;
        }

        [[nodiscard]]
        virtual auto context2D() noexcept -> rhi::Context2D* {
            if(auto* const n = findUp<RenderNode>()) {
                return n->context2D();
            }
            return nullptr;
        }

        [[nodiscard]]
        virtual auto context3D() noexcept -> rhi::Context3D* {
            if(auto* const n = findUp<RenderNode>()) {
                return n->context3D();
            }
            return nullptr;
        }
    };

}

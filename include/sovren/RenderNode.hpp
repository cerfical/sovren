#pragma once

#include <sovren/pal/Window.hpp>

#include <sovren/rhi/Context2D.hpp>
#include <sovren/rhi/Context3D.hpp>
#include <sovren/rhi/Device.hpp>
#include <sovren/rhi/Texture.hpp>

#include <sovren/SceneNode.hpp>

namespace sovren {

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
        virtual auto target() noexcept -> rhi::Texture* {
            if(auto* const n = findUp<RenderNode>()) {
                return n->target();
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

        [[nodiscard]]
        virtual auto window() noexcept -> Window* {
            if(auto* const n = findUp<RenderNode>()) {
                return n->window();
            }
            return nullptr;
        }
    };

}

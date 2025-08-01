#pragma once

#include "pal/Window.hpp"

#include "rhi/Context2D.hpp"
#include "rhi/Device.hpp"
#include "rhi/Texture.hpp"

#include "SceneNode.hpp"

namespace sovren {

    class RenderNode : public SceneNode {
    public:
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
        virtual auto window() noexcept -> Window* {
            if(auto* const n = findUp<RenderNode>()) {
                return n->window();
            }
            return nullptr;
        }
    };

}

#pragma once

#include "Render2D.hpp"
#include "Render3D.hpp"
#include "SwapChain.hpp"
#include "VertexBuffer.hpp"

#include <memory>
#include <span>

namespace sovren {

    class RenderBackend {
    public:

        static auto create() -> std::unique_ptr<RenderBackend>;


        RenderBackend() = default;

        RenderBackend(const RenderBackend&) = delete;
        auto operator=(const RenderBackend&) -> RenderBackend& = delete;

        RenderBackend(RenderBackend&&) = delete;
        auto operator=(RenderBackend&&) -> RenderBackend& = delete;

        virtual ~RenderBackend() = default;


        virtual auto createRender2D() -> std::unique_ptr<Render2D> = 0;

        virtual auto createRender3D() -> std::unique_ptr<Render3D> = 0;

        virtual auto createSwapChain(void* window) -> std::unique_ptr<SwapChain> = 0;

        virtual auto createVertexBuffer(std::span<const std::byte> data) -> std::unique_ptr<VertexBuffer> = 0;
    };

}

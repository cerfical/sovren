#pragma once

#include "RenderContext.hpp"
#include "SwapChain.hpp"
#include "VertexBuffer.hpp"

#include <memory>
#include <span>

namespace sovren::rhi {

    class RenderBackend {
    public:

        static auto create() -> std::unique_ptr<RenderBackend>;


        RenderBackend() = default;

        RenderBackend(const RenderBackend&) = delete;
        auto operator=(const RenderBackend&) -> RenderBackend& = delete;

        RenderBackend(RenderBackend&&) = delete;
        auto operator=(RenderBackend&&) -> RenderBackend& = delete;

        virtual ~RenderBackend() = default;


        virtual auto createSwapChain(void* window) -> std::unique_ptr<SwapChain> = 0;

        virtual auto createRenderContext() -> std::unique_ptr<RenderContext> = 0;

        virtual auto createVertexBuffer(std::span<const std::byte> data) -> std::unique_ptr<VertexBuffer> = 0;
    };

}

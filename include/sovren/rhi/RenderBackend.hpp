#pragma once

#include "RenderContext.hpp"
#include "SwapChain.hpp"
#include "VertexBuffer.hpp"

#include <memory>
#include <span>

namespace sovren::rhi {

    class RenderBackend {
    public:

        RenderBackend(const RenderBackend&) = delete;
        RenderBackend& operator=(const RenderBackend&) = delete;

        RenderBackend(RenderBackend&&) = delete;
        RenderBackend& operator=(RenderBackend&&) = delete;

        RenderBackend() = default;
        virtual ~RenderBackend() = default;


        virtual std::unique_ptr<SwapChain> createSwapChain(void* window) = 0;

        virtual std::unique_ptr<RenderContext> createRenderContext() = 0;

        virtual std::unique_ptr<VertexBuffer> createVertexBuffer(std::span<const std::byte> data) = 0;
    };

}

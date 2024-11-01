#pragma once

#include "../WindowHandle.hpp"

#include "RenderContext.hpp"
#include "SwapChain.hpp"
#include "VertexBuffer.hpp"

#include <memory>
#include <span>

namespace sovren::rhi {

    class RenderDevice {
    public:

        static auto create() -> std::unique_ptr<RenderDevice>;


        RenderDevice() = default;

        RenderDevice(const RenderDevice&) = delete;
        auto operator=(const RenderDevice&) -> RenderDevice& = delete;

        RenderDevice(RenderDevice&&) = delete;
        auto operator=(RenderDevice&&) -> RenderDevice& = delete;

        virtual ~RenderDevice() = default;


        [[nodiscard]]
        virtual auto createRenderContext() -> std::unique_ptr<RenderContext> = 0;


        [[nodiscard]]
        virtual auto createSwapChain(WindowHandle window) -> std::unique_ptr<SwapChain> = 0;


        [[nodiscard]]
        virtual auto createVertexBuffer(std::span<const std::byte> data) -> std::unique_ptr<VertexBuffer> = 0;
    };

}

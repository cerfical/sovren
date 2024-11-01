#pragma once

#include "../WindowHandle.hpp"

#include "RenderContext2D.hpp"
#include "RenderContext3D.hpp"
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
        virtual auto createRenderContext2D() -> std::unique_ptr<RenderContext2D> = 0;


        [[nodiscard]]
        virtual auto createRenderContext3D() -> std::unique_ptr<RenderContext3D> = 0;


        [[nodiscard]]
        virtual auto createSwapChain(WindowHandle window) -> std::unique_ptr<SwapChain> = 0;


        [[nodiscard]]
        virtual auto createVertexBuffer(std::span<const std::byte> data) -> std::unique_ptr<VertexBuffer> = 0;
    };

}

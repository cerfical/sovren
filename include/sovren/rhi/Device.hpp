#pragma once

#include <sovren/rhi/Buffer.hpp>
#include <sovren/rhi/BufferDesc.hpp>
#include <sovren/rhi/Context2D.hpp>
#include <sovren/rhi/Context3D.hpp>
#include <sovren/rhi/Pipeline.hpp>
#include <sovren/rhi/PipelineDesc.hpp>
#include <sovren/rhi/SwapChain.hpp>

#include <sovren/util/Handle.hpp>

#include <memory>

namespace sovren::rhi {

    class Device {
    public:
        Device() = default;

        Device(const Device&) = delete;
        auto operator=(const Device&) -> Device& = delete;

        Device(Device&&) = delete;
        auto operator=(Device&&) -> Device& = delete;

        virtual ~Device() = default;

        [[nodiscard]]
        static auto create() -> std::unique_ptr<Device>;

        [[nodiscard]]
        virtual auto createSwapChain(Handle window) -> std::unique_ptr<SwapChain> = 0;

        [[nodiscard]]
        virtual auto createContext2D() -> std::unique_ptr<Context2D> = 0;

        [[nodiscard]]
        virtual auto createContext3D() -> std::unique_ptr<Context3D> = 0;

        [[nodiscard]]
        virtual auto createPipeline(const PipelineDesc&) -> std::unique_ptr<Pipeline> = 0;

        [[nodiscard]]
        virtual auto createBuffer(const BufferDesc&) -> std::unique_ptr<Buffer> = 0;
    };

}

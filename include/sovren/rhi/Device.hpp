#pragma once

#include "../core/Handle.hpp"

#include "Context2D.hpp"
#include "SwapChain.hpp"

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
    };

}

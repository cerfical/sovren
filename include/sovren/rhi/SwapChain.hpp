#pragma once

#include "../core/Size2.hpp"

#include "Texture.hpp"

namespace sovren::rhi {

    class SwapChain {
    public:
        SwapChain() = default;

        SwapChain(const SwapChain&) = delete;
        auto operator=(const SwapChain&) -> SwapChain& = delete;

        SwapChain(SwapChain&&) = delete;
        auto operator=(SwapChain&&) -> SwapChain& = delete;

        virtual ~SwapChain() = default;

        [[nodiscard]]
        virtual auto frontBuffer() -> Texture* = 0;

        virtual void resizeBuffers(Size2i) = 0;

        virtual void swapBuffers() = 0;
    };

}

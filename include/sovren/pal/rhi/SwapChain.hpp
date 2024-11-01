#pragma once

#include "../../Size2.hpp"
#include "RenderTarget.hpp"

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
        virtual auto frontBuffer() -> RenderTarget& = 0;


        virtual void swapBuffers() = 0;


        virtual void setBufferSize(Size2 newSize) = 0;


        [[nodiscard]]
        virtual auto bufferSize() const -> Size2 = 0;
    };

}

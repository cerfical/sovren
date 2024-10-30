#pragma once

#include "../Size2.hpp"

namespace sovren {

    class RenderTarget;


    class SwapChain {
    public:

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator=(const SwapChain&) = delete;

        SwapChain(SwapChain&&) = delete;
        SwapChain& operator=(SwapChain&&) = delete;

        SwapChain() = default;
        virtual ~SwapChain() = default;


        virtual RenderTarget& frontBuffer() = 0;

        virtual void swapBuffers() = 0;


        virtual void setBufferSize(Size2 newSize) = 0;

        virtual Size2 bufferSize() const = 0;
    };

}

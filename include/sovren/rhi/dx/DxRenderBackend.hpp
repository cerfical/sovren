#pragma once

#include "../RenderBackend.hpp"
#include "util.hpp"

#include <d2d1_1.h>
#include <d3d11.h>

namespace sovren::rhi::dx {

    class DxRenderBackend : public RenderBackend {
    public:

        DxRenderBackend();


        std::unique_ptr<SwapChain> createSwapChain(void* window) override;

        std::unique_ptr<RenderContext> createRenderContext() override;

        std::unique_ptr<VertexBuffer> createVertexBuffer(std::span<const std::byte> data) override;


    private:
        static constexpr UINT d3dDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
                                             | D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support
#if not defined(NDEBUG) || defined(_DEBUG)
                                             | D3D11_CREATE_DEVICE_DEBUG
#endif
            ;

        static constexpr D2D1_FACTORY_OPTIONS d2dFactoryOptions = {
#if not defined(NDEBUG) || defined(_DEBUG)
            D2D1_DEBUG_LEVEL_INFORMATION
#else
            D2D1_DEBUG_LEVEL_NONE
#endif
        };

        ComPtr<ID3D11Device> m_d3dDevice;
        ComPtr<ID2D1Factory1> m_d2dFactory;

        ComPtr<ID2D1Device> m_d2dDevice;
        ComPtr<ID2D1DeviceContext> m_d2dContext;
    };

}

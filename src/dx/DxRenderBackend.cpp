#include "rhi/dx/DxRenderBackend.hpp"

#include "rhi/dx/DxRenderContext.hpp"
#include "rhi/dx/DxSwapChain.hpp"
#include "rhi/dx/DxVertexBuffer.hpp"

namespace sovren::rhi::dx {
    DxRenderBackend::DxRenderBackend() {
        comCheck(D3D11CreateDevice(
            nullptr, // use default IDXGIAdapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr, // hardware driver
            d3dDeviceFlags,
            nullptr,
            0, // default feature levels
            D3D11_SDK_VERSION,
            &m_d3dDevice,
            nullptr, // no need to determine the available feature level
            nullptr
        ));

        comCheck(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactoryOptions, &m_d2dFactory));


        ComPtr<IDXGIDevice> dxgiDevice;
        comCheck(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

        comCheck(m_d2dFactory->CreateDevice(dxgiDevice, &m_d2dDevice));
        comCheck(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext));
    }


    std::unique_ptr<SwapChain> DxRenderBackend::createSwapChain(void* window) {
        return std::make_unique<DxSwapChain>(static_cast<HWND>(window), m_d3dDevice, m_d2dContext);
    }


    std::unique_ptr<RenderContext> DxRenderBackend::createRenderContext() {
        ComPtr<ID3D11DeviceContext> d3dContext;
        m_d3dDevice->GetImmediateContext(&d3dContext);

        return std::make_unique<DxRenderContext>(m_d2dContext, d3dContext);
    }


    std::unique_ptr<VertexBuffer> DxRenderBackend::createVertexBuffer(std::span<const std::byte> data) {
        return std::make_unique<DxVertexBuffer>(data, m_d3dDevice);
    }
}

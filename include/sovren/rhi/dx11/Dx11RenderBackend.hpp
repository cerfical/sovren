#pragma once

#include "../RenderBackend.hpp"

#include "Dx11RenderContext.hpp"
#include "Dx11SwapChain.hpp"
#include "Dx11VertexBuffer.hpp"

#include "com_util.hpp"
#include "config.hpp"

#include <d2d1_1.h>
#include <d3d11.h>

namespace sovren::dx11 {

    class Dx11RenderBackend : public RenderBackend {
    public:

        Dx11RenderBackend() {
            comCheck(D3D11CreateDevice(
                nullptr, // use default IDXGIAdapter
                D3D_DRIVER_TYPE_HARDWARE,
                nullptr, // hardware driver
                D3dDeviceFlags,
                nullptr,
                0, // default feature levels
                D3D11_SDK_VERSION,
                &d3dDevice_,
                nullptr, // no need to determine the available feature level
                nullptr
            ));

            comCheck(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, D2dFactoryOptions, &d2dFactory_));


            ComPtr<IDXGIDevice> dxgiDevice;
            comCheck(d3dDevice_->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

            comCheck(d2dFactory_->CreateDevice(dxgiDevice, &d2dDevice_));
            comCheck(d2dDevice_->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext_));
        }


        [[nodiscard]]
        auto createSwapChain(void* window) -> std::unique_ptr<SwapChain> override {
            return std::make_unique<Dx11SwapChain>(static_cast<HWND>(window), d3dDevice_, d2dContext_);
        }


        [[nodiscard]]
        auto createRenderContext() -> std::unique_ptr<RenderContext> override {
            ComPtr<ID3D11DeviceContext> d3dContext;
            d3dDevice_->GetImmediateContext(&d3dContext);

            return std::make_unique<Dx11RenderContext>(d2dContext_, d3dContext);
        }


        [[nodiscard]]
        auto createVertexBuffer(std::span<const std::byte> data) -> std::unique_ptr<VertexBuffer> override {
            return std::make_unique<Dx11VertexBuffer>(data, d3dDevice_);
        }


    private:
        static constexpr UINT D3dDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
                                             | D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support
                                             | (Config::isDebug() ? D3D11_CREATE_DEVICE_DEBUG : 0);

        static constexpr D2D1_FACTORY_OPTIONS D2dFactoryOptions = {
            (Config::isDebug() ? D2D1_DEBUG_LEVEL_INFORMATION : D2D1_DEBUG_LEVEL_NONE)
        };


        ComPtr<ID3D11Device> d3dDevice_;
        ComPtr<ID2D1Factory1> d2dFactory_;

        ComPtr<ID2D1Device> d2dDevice_;
        ComPtr<ID2D1DeviceContext> d2dContext_;
    };

}

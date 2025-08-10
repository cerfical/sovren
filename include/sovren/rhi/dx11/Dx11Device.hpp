#pragma once

#include <sovren/rhi/Device.hpp>

#include <sovren/rhi/dx11/Dx11Buffer.hpp>
#include <sovren/rhi/dx11/Dx11Context2D.hpp>
#include <sovren/rhi/dx11/Dx11Context3D.hpp>
#include <sovren/rhi/dx11/Dx11Pipeline.hpp>
#include <sovren/rhi/dx11/Dx11SwapChain.hpp>

#include <sovren/config.hpp>

#include <d2d1_1.h>
#include <d3d11.h>

#include <memory>

namespace sovren::rhi {

    class Dx11Device final : public Device {
    public:
        Dx11Device() {
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
        }

        auto createSwapChain(Handle window) -> std::unique_ptr<SwapChain> override {
            return std::make_unique<Dx11SwapChain>(static_cast<HWND>(window), d3dDevice_);
        }

        auto createContext2D() -> std::unique_ptr<Context2D> override {
            ComPtr<IDXGIDevice> dxgiDevice;
            comCheck(d3dDevice_->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

            ComPtr<ID2D1Factory1> d2dFactory;
            comCheck(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, D2dFactoryOptions, &d2dFactory));

            ComPtr<ID2D1Device> d2dDevice;
            comCheck(d2dFactory->CreateDevice(dxgiDevice, &d2dDevice));

            ComPtr<ID2D1DeviceContext> d2dContext;
            comCheck(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &d2dContext));
            return std::make_unique<Dx11Context2D>(d2dContext);
        }

        auto createContext3D() -> std::unique_ptr<Context3D> override {
            ComPtr<ID3D11DeviceContext> d3dContext;
            d3dDevice_->GetImmediateContext(&d3dContext);
            return std::make_unique<Dx11Context3D>(d3dContext);
        }

        auto createPipeline(const PipelineDesc& desc) -> std::unique_ptr<Pipeline> override {
            return std::make_unique<Dx11Pipeline>(desc, d3dDevice_);
        }

        auto createBuffer(const BufferDesc& desc) -> std::unique_ptr<Buffer> override {
            return std::make_unique<Dx11Buffer>(desc, d3dDevice_);
        }

    private:
        static constexpr UINT D3dDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
            | D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support
            | (Config::isDebug() ? D3D11_CREATE_DEVICE_DEBUG : 0);

        static constexpr D2D1_FACTORY_OPTIONS D2dFactoryOptions = {
            (Config::isDebug() ? D2D1_DEBUG_LEVEL_INFORMATION : D2D1_DEBUG_LEVEL_NONE)
        };

        ComPtr<ID3D11Device> d3dDevice_;
    };

}

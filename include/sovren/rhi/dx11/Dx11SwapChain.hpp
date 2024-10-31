#pragma once

#include "../../Size2.hpp"
#include "../SwapChain.hpp"

#include "Dx11RenderTarget.hpp"
#include "com_util.hpp"

#include <d2d1_1.h>
#include <d3d11.h>
#include <dxgi1_2.h>

#include <optional>

namespace sovren::dx11 {

    class Dx11SwapChain : public SwapChain {
    public:

        Dx11SwapChain(HWND window, ID3D11Device* d3dDevice, ID2D1DeviceContext* d2dContext)
            : d2dContext_(d2dContext) {

            // retrieve the IDXGIFactory2 interface associated with the ID3D11Device
            ComPtr<IDXGIDevice> dxgiDevice;
            comCheck(d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

            ComPtr<IDXGIAdapter> dxgiAdapter;
            comCheck(dxgiDevice->GetAdapter(&dxgiAdapter));

            ComPtr<IDXGIFactory2> dxgiFactory;
            comCheck(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

            // initialize and create a swap chain
            DXGI_SWAP_CHAIN_DESC1 scDesc = {
                .Width = 0, // make the buffer size be equal to the window size
                .Height = 0,
                .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
                .Stereo = FALSE,
                .SampleDesc = { .Count = 1, .Quality = 0 },
                .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
                .BufferCount = 2,
                .Scaling = DXGI_SCALING_NONE,
                .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
                .AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
                .Flags = 0
            };

            ComPtr<IDXGISwapChain1> swapChain;
            comCheck(dxgiFactory->CreateSwapChainForHwnd(
                d3dDevice,
                window, // output window
                &scDesc,
                nullptr, // no fullscreen description
                nullptr, // not restricted to some IDXGIOutput
                &swapChain
            ));
            comCheck(swapChain->GetDesc1(&scDesc));

            swapChain_ = swapChain;
            setBufferSize({ .width = static_cast<int>(scDesc.Width), .height = static_cast<int>(scDesc.Height) });
        }


        [[nodiscard]]
        auto frontBuffer() -> RenderTarget& override {
            return frontBuffer_.value();
        }


        void swapBuffers() override {
            comCheck(swapChain_->Present(0, 0));
        }


        void setBufferSize(Size2 newSize) override {
            frontBuffer_.reset();
            comCheck(swapChain_->ResizeBuffers(
                0, // preserve the existing number of buffers
                static_cast<UINT>(newSize.width),
                static_cast<UINT>(newSize.height),
                DXGI_FORMAT_UNKNOWN, // no change to buffer format
                0                    // no flags
            ));

            ComPtr<IDXGISurface> frontBuffer;
            comCheck(swapChain_->GetBuffer(0, IID_PPV_ARGS(&frontBuffer)));
            frontBuffer_.emplace(frontBuffer, d2dContext_);

            bufferSize_ = newSize;
        }


        [[nodiscard]]
        auto bufferSize() const -> Size2 override {
            return bufferSize_;
        }


    private:
        std::optional<Dx11RenderTarget> frontBuffer_;
        Size2 bufferSize_;

        ComPtr<IDXGISwapChain> swapChain_;
        ComPtr<ID2D1DeviceContext> d2dContext_;
    };

}

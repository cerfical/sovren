#pragma once

#include <sovren/rhi/SwapChain.hpp>

#include <sovren/rhi/dx11/Dx11Texture.hpp>
#include <sovren/rhi/dx11/com_util.hpp>

#include <sovren/util/Size2.hpp>

#include <d3d11.h>
#include <dxgi1_2.h>

#include <optional>

namespace sovren::rhi {

    class Dx11SwapChain final : public SwapChain {
    public:
        explicit Dx11SwapChain(HWND window, ID3D11Device* device) {
            const DXGI_SWAP_CHAIN_DESC1 scDesc = {
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

            ComPtr<IDXGIDevice> dxgiDevice;
            comCheck(device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

            ComPtr<IDXGIAdapter> dxgiAdapter;
            comCheck(dxgiDevice->GetAdapter(&dxgiAdapter));

            ComPtr<IDXGIFactory2> dxgiFactory;
            comCheck(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

            comCheck(dxgiFactory->CreateSwapChainForHwnd(
                device,
                window, // output window
                &scDesc,
                nullptr, // no fullscreen description
                nullptr, // not restricted to some IDXGIOutput
                &swapChain_
            ));

            frontBuffer_.emplace(getFrontBuffer());
        }

        [[nodiscard]]
        auto frontBuffer() -> Texture* override {
            return &frontBuffer_.value();
        }

        void resizeBuffers(Size2i size) override {
            frontBuffer_.reset();

            comCheck(swapChain_->ResizeBuffers(
                0, // preserve the existing number of buffers
                static_cast<UINT>(size.width),
                static_cast<UINT>(size.height),
                DXGI_FORMAT_UNKNOWN, // no change to buffer format
                0                    // no flags
            ));

            frontBuffer_.emplace(getFrontBuffer());
        }

        void swapBuffers() override {
            comCheck(swapChain_->Present(0, 0));
        }

    private:
        [[nodiscard]]
        auto getFrontBuffer() const -> ComPtr<ID3D11Texture2D> {
            ComPtr<ID3D11Texture2D> frontBuffer;
            comCheck(swapChain_->GetBuffer(0, IID_PPV_ARGS(&frontBuffer)));
            return frontBuffer;
        }

        std::optional<Dx11Texture> frontBuffer_;
        ComPtr<IDXGISwapChain1> swapChain_;
    };

}

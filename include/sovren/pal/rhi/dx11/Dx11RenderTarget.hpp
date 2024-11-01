#pragma once

#include "../RenderTarget.hpp"

#include "com_util.hpp"

#include <d2d1_1.h>
#include <d3d11.h>

namespace sovren::rhi::dx11 {

    class Dx11RenderTarget : public RenderTarget {
    public:

        Dx11RenderTarget(IDXGISurface* renderSurface, ID2D1DeviceContext* d2dContext) {
            const auto bitmapProps = D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
                0,
                0
            );

            // allocate Direct2D render target
            ComPtr<ID2D1Bitmap1> d2dRenderBitmap;
            comCheck(d2dContext->CreateBitmapFromDxgiSurface(renderSurface, bitmapProps, &d2dRenderBitmap));
            d2dRenderImage_ = d2dRenderBitmap;


            // create Direct3D render target and depth stencil views
            ComPtr<ID3D11Texture2D> renderTexture;
            comCheck(renderSurface->QueryInterface(IID_PPV_ARGS(&renderTexture)));

            ComPtr<ID3D11Device> d3dDevice;
            renderTexture->GetDevice(&d3dDevice);
            comCheck(d3dDevice->CreateRenderTargetView(renderTexture, nullptr, &d3dRenderView_));

            D3D11_TEXTURE2D_DESC depthDesc = {};
            renderTexture->GetDesc(&depthDesc);
            depthDesc = {
                .Width = depthDesc.Width,
                .Height = depthDesc.Height,
                .MipLevels = 1,
                .ArraySize = 1,
                .Format = DXGI_FORMAT_D32_FLOAT,
                .SampleDesc = { .Count = 1, .Quality = 0 },
                .Usage = D3D11_USAGE_DEFAULT,
                .BindFlags = D3D11_BIND_DEPTH_STENCIL,
                .CPUAccessFlags = 0,
                .MiscFlags = 0
            };

            ComPtr<ID3D11Texture2D> depthTexture;
            comCheck(d3dDevice->CreateTexture2D(&depthDesc, nullptr, &depthTexture));
            comCheck(d3dDevice->CreateDepthStencilView(depthTexture, nullptr, &d3dDepthView_));

            width_ = depthDesc.Width;
            height_ = depthDesc.Height;
        }


        [[nodiscard]]
        auto asImage() const noexcept -> ID2D1Image* {
            return d2dRenderImage_;
        }


        [[nodiscard]]
        auto asRenderView() const noexcept -> ID3D11RenderTargetView* {
            return d3dRenderView_;
        }


        [[nodiscard]]
        auto asDepthView() const noexcept -> ID3D11DepthStencilView* {
            return d3dDepthView_;
        }


        [[nodiscard]]
        auto width() const noexcept -> UINT {
            return width_;
        }


        [[nodiscard]]
        auto height() const noexcept -> UINT {
            return height_;
        }


    private:
        ComPtr<ID2D1Image> d2dRenderImage_;

        ComPtr<ID3D11RenderTargetView> d3dRenderView_;
        ComPtr<ID3D11DepthStencilView> d3dDepthView_;

        UINT width_ = {};
        UINT height_ = {};
    };

}

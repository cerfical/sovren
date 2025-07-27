#pragma once

#include "../Texture.hpp"
#include "com_util.hpp"

#include <d2d1_1.h>
#include <d3d11.h>

namespace sovren::rhi {

    class Dx11Texture final : public Texture {
    public:
        Dx11Texture() = default;

        explicit Dx11Texture(ID3D11Texture2D* texture)
            : texture_(texture) {
        }

        void setAsRenderTarget(ID2D1DeviceContext* d2dContext) {
            if(!image_ && texture_) {
                ComPtr<IDXGISurface> surface;
                comCheck(texture_->QueryInterface(IID_PPV_ARGS(&surface)));

                const auto bitmapProps = D2D1::BitmapProperties1(
                    D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                    D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
                    0,
                    0
                );
                comCheck(d2dContext->CreateBitmapFromDxgiSurface(surface, bitmapProps, &image_));
            }
            d2dContext->SetTarget(image_);
        }

    private:
        ComPtr<ID3D11Texture2D> texture_;
        ComPtr<ID2D1Bitmap1> image_;
    };

}

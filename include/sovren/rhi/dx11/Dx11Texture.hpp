#pragma once

#include "../Texture.hpp"
#include "com_util.hpp"

#include <d2d1_1.h>
#include <d3d11.h>

namespace sovren::rhi {

    class Dx11Texture : public Texture {
    public:
        explicit Dx11Texture(ComPtr<ID3D11Texture2D> texture)
            : texture(std::move(texture)) {}

        ComPtr<ID3D11Texture2D> texture;
        ComPtr<ID3D11RenderTargetView> rtv;

        ComPtr<ID2D1Bitmap1> image;
    };

}

#pragma once

#include "../Context2D.hpp"

#include "Dx11Texture.hpp"
#include "com_util.hpp"

#include <d2d1.h>
#include <d2d1_1.h>

namespace sovren::rhi {

    class Dx11Context2D : public Context2D {
    public:
        explicit Dx11Context2D(ComPtr<ID2D1DeviceContext> d2dContext)
            : d2dContext_(std::move(d2dContext)) {
            comCheck(d2dContext_->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &drawBrush_));
        }

        void setRenderTarget(Texture* rt) override {
            ID2D1Image* image = nullptr;
            if(rt) {
                auto& dxt = dynamic_cast<Dx11Texture&>(*rt);
                if(!dxt.image) {
                    dxt.image = createBitmap(dxt.texture);
                }
                image = dxt.image;
            }
            d2dContext_->SetTarget(image);
        }

        void setTransform(const std::array<std::array<float, 2>, 3>& m) override {
            d2dContext_->SetTransform({
                m[0][0],
                m[0][1],
                m[1][0],
                m[1][1],
                m[2][0],
                m[2][1],
            });
        }

        void setOutlineColor(Color c) override {
            drawBrush_->SetColor(static_cast<D2D1_COLOR_F>(c));
        }

        void beginDraw() override {
            d2dContext_->BeginDraw();
        }

        void endDraw() override {
            d2dContext_->EndDraw();
        }

        void clear(Color c) override {
            d2dContext_->Clear(static_cast<D2D1_COLOR_F>(c));
        }

        void drawLine(Point2f start, Point2f end) override {
            d2dContext_->DrawLine(
                static_cast<D2D1_POINT_2F>(start),
                static_cast<D2D1_POINT_2F>(end),
                drawBrush_
            );
        }

        void drawRect(Point2f topLeft, Point2f botRight) override {
            d2dContext_->DrawRectangle(
                { topLeft.x, topLeft.y, botRight.x, botRight.y },
                drawBrush_
            );
        }

    private:
        auto createBitmap(ID3D11Texture2D* texture) -> ComPtr<ID2D1Bitmap1> {
            ComPtr<IDXGISurface> surface;
            comCheck(texture->QueryInterface(IID_PPV_ARGS(&surface)));

            const auto bitmapProps = D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
                D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
                0,
                0
            );

            ComPtr<ID2D1Bitmap1> image;
            comCheck(d2dContext_->CreateBitmapFromDxgiSurface(surface, bitmapProps, &image));
            return image;
        }

        ComPtr<ID2D1DeviceContext> d2dContext_;
        ComPtr<ID2D1SolidColorBrush> drawBrush_;
    };

}

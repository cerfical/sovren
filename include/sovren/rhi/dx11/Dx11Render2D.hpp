#pragma once

#include "../Render2D.hpp"

#include "Dx11RenderTarget.hpp"
#include "com_util.hpp"

#include <d2d1_1.h>

namespace sovren::dx11 {

    class Dx11Render2D : public Render2D {
    public:

        explicit Dx11Render2D(ID2D1DeviceContext* d2dContext)
            : d2dContext_(d2dContext) {
            comCheck(d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &drawBrush_));
        }


        void startRender(RenderTarget& rt) override {
            d2dContext_->SetTarget(dynamic_cast<Dx11RenderTarget&>(rt).asImage());
            d2dContext_->BeginDraw();
        }


        void endRender() override {
            comCheck(d2dContext_->EndDraw());
            d2dContext_->SetTarget(nullptr);
        }


        void drawLine(Vec2 start, Vec2 end) override {
            d2dContext_->DrawLine({ start.x, start.y }, { end.x, end.y }, drawBrush_);
        }


        void drawRect(Vec2 topLeft, Vec2 borromRight) override {
            d2dContext_->DrawRectangle({ topLeft.x, topLeft.y, borromRight.x, borromRight.y }, drawBrush_);
        }


        void clear(Color col) override {
            const auto colVec = col.toVec();
            d2dContext_->Clear({ colVec.x, colVec.y, colVec.z, colVec.w });
        }


        void setTransform(const Mat3x3& mat) override {
            // Direct2D only supports affine transformations, so ignore the last column values
            d2dContext_->SetTransform({ { { mat[0][0], mat[0][1], mat[1][0], mat[1][1], mat[2][0], mat[2][1] } } });
        }


    private:
        ComPtr<ID2D1DeviceContext> d2dContext_;
        ComPtr<ID2D1SolidColorBrush> drawBrush_;
    };

}

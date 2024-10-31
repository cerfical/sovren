#pragma once

#include "../Painter.hpp"

#include "Dx11RenderTarget.hpp"
#include "com_util.hpp"

#include <d2d1_1.h>

namespace sovren::dx11 {

    class Dx11Painter : public Painter {
    public:

        explicit Dx11Painter(ID2D1DeviceContext* d2dContext)
            : d2dContext_(d2dContext) {
            comCheck(d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &drawBrush_));
        }


        void drawLine(Vec2 start, Vec2 end) override {
            d2dContext_->DrawLine({ start.x, start.y }, { end.x, end.y }, drawBrush_);
        }


        void drawRect(Vec2 topLeft, Vec2 borromRight) override {
            d2dContext_->DrawRectangle({ topLeft.x, topLeft.y, borromRight.x, borromRight.y }, drawBrush_);
        }


        void setTransform(const Mat3x3& mat) override {
            // Direct2D only supports affine transformations, so ignore the last column values
            d2dContext_->SetTransform({ { { mat[0][0], mat[0][1], mat[1][0], mat[1][1], mat[2][0], mat[2][1] } } });
        }


        void setTarget(Dx11RenderTarget& rt) {
            d2dContext_->SetTarget(rt.asImage());
        }


        void startDraw() {
            d2dContext_->BeginDraw();
        }


        void endDraw() {
            comCheck(d2dContext_->EndDraw());
            d2dContext_->SetTarget(nullptr);
        }


    private:
        ComPtr<ID2D1DeviceContext> d2dContext_;
        ComPtr<ID2D1SolidColorBrush> drawBrush_;
    };

}

#pragma once

#include "../../math/Mat3x3.hpp"
#include "../../math/Vec2.hpp"

#include "RenderTarget.hpp"

namespace sovren::rhi {

    class RenderContext2D {
    public:

        RenderContext2D() = default;

        RenderContext2D(const RenderContext2D&) = delete;
        auto operator=(const RenderContext2D&) -> RenderContext2D& = delete;

        RenderContext2D(RenderContext2D&&) = delete;
        auto operator=(RenderContext2D&&) -> RenderContext2D& = delete;

        virtual ~RenderContext2D() = default;


        virtual void setTarget(RenderTarget& rt) = 0;

        virtual void startDraw() = 0;

        virtual void endDraw() = 0;


        virtual void drawLine(Vec2 start, Vec2 end) = 0;

        virtual void drawRect(Vec2 topLeft, Vec2 bottomRight) = 0;

        virtual void setTransform(const Mat3x3& mat) = 0;
    };

}

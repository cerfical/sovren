#pragma once

#include "../Color.hpp"
#include "../math/Mat3x3.hpp"
#include "../math/Vec2.hpp"

#include "RenderTarget.hpp"

namespace sovren {

    class Render2D {
    public:

        Render2D() = default;

        Render2D(const Render2D&) = delete;
        auto operator=(const Render2D&) -> Render2D& = delete;

        Render2D(Render2D&&) = delete;
        auto operator=(Render2D&&) -> Render2D& = delete;

        virtual ~Render2D() = default;


        virtual void startRender(RenderTarget& rt) = 0;

        virtual void endRender() = 0;


        virtual void drawLine(Vec2 start, Vec2 end) = 0;

        virtual void drawRect(Vec2 topLeft, Vec2 bottomRight) = 0;

        virtual void clear(Color col) = 0;


        virtual void setTransform(const Mat3x3& mat) = 0;
    };

}

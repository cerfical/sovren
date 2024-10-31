#pragma once

#include "../math/Mat3x3.hpp"
#include "../math/Vec2.hpp"

namespace sovren {

    class Painter {
    public:

        Painter() = default;

        Painter(const Painter&) = delete;
        auto operator=(const Painter&) -> Painter& = delete;

        Painter(Painter&&) = delete;
        auto operator=(Painter&&) -> Painter& = delete;

        virtual ~Painter() = default;


        virtual void drawLine(Vec2 start, Vec2 end) = 0;

        virtual void drawRect(Vec2 topLeft, Vec2 bottomRight) = 0;

        virtual void setTransform(const Mat3x3& mat) = 0;
    };

}

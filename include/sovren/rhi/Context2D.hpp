#pragma once

#include <sovren/rhi/Texture.hpp>

#include <sovren/util/Color.hpp>
#include <sovren/util/Point2.hpp>

#include <array>

namespace sovren::rhi {

    class Context2D {
    public:
        Context2D() = default;

        Context2D(const Context2D&) = delete;
        auto operator=(const Context2D&) -> Context2D& = delete;

        Context2D(Context2D&&) = delete;
        auto operator=(Context2D&&) -> Context2D& = delete;

        virtual ~Context2D() = default;

        virtual void setRenderTarget(Texture*) = 0;

        virtual void setTransform(const std::array<std::array<float, 2>, 3>&) = 0;

        virtual void setOutlineColor(Color) = 0;

        virtual void beginDraw() = 0;

        virtual void endDraw() = 0;

        virtual void clear(Color) = 0;

        virtual void drawLine(Point2f start, Point2f end) = 0;

        virtual void drawRect(Point2f topLeft, Point2f botRight) = 0;
    };

}

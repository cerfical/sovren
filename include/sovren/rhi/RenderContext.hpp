#pragma once

#include "../Color.hpp"
#include "../math/Mat4x4.hpp"

#include "Painter.hpp"
#include "RenderTarget.hpp"
#include "VertexBuffer.hpp"

namespace sovren {

    class RenderContext {
    public:

        RenderContext() = default;

        RenderContext(const RenderContext&) = delete;
        auto operator=(const RenderContext&) -> RenderContext& = delete;

        RenderContext(RenderContext&&) = delete;
        auto operator=(RenderContext&&) -> RenderContext& = delete;

        virtual ~RenderContext() = default;


        virtual void setTarget(RenderTarget& rt) = 0;

        virtual void startDraw() = 0;

        virtual void endDraw() = 0;


        virtual void setTransform(const Mat4x4& mat) = 0;

        virtual void drawMesh(const VertexBuffer& vert) = 0;

        virtual void clear(Color col) = 0;


        [[nodiscard]]
        virtual auto painter() -> Painter& = 0;
    };

}

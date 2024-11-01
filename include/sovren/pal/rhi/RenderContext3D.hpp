#pragma once

#include "../../Color.hpp"
#include "../../math/Mat4x4.hpp"

#include "RenderTarget.hpp"
#include "VertexBuffer.hpp"

namespace sovren::rhi {

    class RenderContext3D {
    public:

        RenderContext3D() = default;

        RenderContext3D(const RenderContext3D&) = delete;
        auto operator=(const RenderContext3D&) -> RenderContext3D& = delete;

        RenderContext3D(RenderContext3D&&) = delete;
        auto operator=(RenderContext3D&&) -> RenderContext3D& = delete;

        virtual ~RenderContext3D() = default;


        virtual void setTarget(RenderTarget& rt) = 0;

        virtual void startDraw() = 0;

        virtual void endDraw() = 0;


        virtual void setTransform(const Mat4x4& mat) = 0;

        virtual void drawMesh(const VertexBuffer& vert) = 0;

        virtual void clear(Color col) = 0;
    };

}

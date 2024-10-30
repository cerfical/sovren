#pragma once

#include "../Color.hpp"
#include "../math/Mat4x4.hpp"

#include "RenderTarget.hpp"
#include "VertexBuffer.hpp"

namespace sovren {

    class Render3D {
    public:

        Render3D() = default;

        Render3D(const Render3D&) = delete;
        auto operator=(const Render3D&) -> Render3D& = delete;

        Render3D(Render3D&&) = delete;
        auto operator=(Render3D&&) -> Render3D& = delete;

        virtual ~Render3D() = default;


        virtual void startRender(RenderTarget& rt) = 0;

        virtual void endRender() = 0;


        virtual void drawMesh(const VertexBuffer& vert) = 0;

        virtual void clear(Color col) = 0;


        virtual void setTransform(const Mat4x4& mat) = 0;
    };

}

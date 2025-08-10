#pragma once

#include "../util/Color.hpp"

#include "AccessModes.hpp"
#include "Buffer.hpp"
#include "Pipeline.hpp"
#include "ShaderStages.hpp"
#include "Texture.hpp"
#include "Viewport.hpp"

#include <cstddef>
#include <span>

namespace sovren::rhi {

    class Context3D {
    public:
        Context3D() = default;

        Context3D(const Context3D&) = delete;
        auto operator=(const Context3D&) -> Context3D& = delete;

        Context3D(Context3D&&) = delete;
        auto operator=(Context3D&&) -> Context3D& = delete;

        virtual ~Context3D() = default;

        virtual void setPipeline(Pipeline*) = 0;

        virtual void setRenderTarget(Texture*) = 0;

        virtual void setVertexBuffer(Buffer*, std::size_t stride) = 0;

        virtual void setConstantBuffer(Buffer*, ShaderStages) = 0;

        virtual void setViewport(const Viewport&) = 0;

        virtual void clear(Color) = 0;

        virtual void draw(std::size_t vertexCount) = 0;

        virtual auto map(Buffer*, AccessModes) -> std::span<std::byte> = 0;

        virtual void unmap(Buffer*) = 0;
    };

}

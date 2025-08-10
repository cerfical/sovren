#pragma once

#include <sovren/rhi/AccessModes.hpp>
#include <sovren/rhi/Buffer.hpp>
#include <sovren/rhi/Pipeline.hpp>
#include <sovren/rhi/ShaderStages.hpp>
#include <sovren/rhi/Texture.hpp>
#include <sovren/rhi/Viewport.hpp>

#include <sovren/util/Color.hpp>

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

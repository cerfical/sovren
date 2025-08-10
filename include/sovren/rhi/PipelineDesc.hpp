#pragma once

#include <sovren/rhi/ShaderCode.hpp>
#include <sovren/rhi/VertexAttr.hpp>

#include <span>

namespace sovren::rhi {

    struct PipelineDesc {
        std::span<const VertexAttr> vertexLayout;

        ShaderCode vertexShader;
        ShaderCode pixelShader;
    };

}

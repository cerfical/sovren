#pragma once

#include "ShaderCode.hpp"
#include "VertexAttr.hpp"

#include <span>

namespace sovren::rhi {

    struct PipelineDesc {
        std::span<const VertexAttr> vertexLayout;

        ShaderCode vertexShader;
        ShaderCode pixelShader;
    };

}

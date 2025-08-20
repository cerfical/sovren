#include <sovren/math/Mat4x4.hpp>

#include <sovren/rhi/AccessModes.hpp>
#include <sovren/rhi/BindModes.hpp>
#include <sovren/rhi/Format.hpp>
#include <sovren/rhi/ShaderLang.hpp>
#include <sovren/rhi/VertexAttr.hpp>

#include <sovren/RenderQueue3D.hpp>

#include <vector>

// TODO: Implement a better way to load shaders
using BYTE = unsigned char;

#include "shaders/ps.hpp"
#include "shaders/vs.hpp"

namespace sovren {
    RenderQueue3D::RenderQueue3D(std::shared_ptr<rhi::Device> device)
        : device_(device) {
        objectBuffer_ = device->createBuffer({
            .size = sizeof(Mat4x4),
            .cpuAccess = rhi::AccessModes::Write,
            .gpuAccess = rhi::AccessModes::Read,
            .bind = rhi::BindModes::ConstantBuffer,
        });

        pipeline_ = device->createPipeline({
            .vertexLayout = std::vector<rhi::VertexAttr>{
                { .name = "position", .format = rhi::Format::Float3 },
            },

            .vertexShader = {
                .data = { reinterpret_cast<const std::byte*>(vs), sizeof(vs) },
                .lang = rhi::ShaderLang::CompiledHLSL,
            },
            .pixelShader = {
                .data = { reinterpret_cast<const std::byte*>(ps), sizeof(ps) },
                .lang = rhi::ShaderLang::CompiledHLSL,
            },
        });
    }
}

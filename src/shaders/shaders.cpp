#include <sovren/rhi/AccessModes.hpp>
#include <sovren/rhi/BindModes.hpp>
#include <sovren/rhi/Format.hpp>
#include <sovren/rhi/ShaderLang.hpp>
#include <sovren/rhi/VertexAttr.hpp>

#include <sovren/MeshNode3D.hpp>

#include <span>
#include <vector>

// TODO: Implement a better way to load shaders
using BYTE = unsigned char;

#include "shaders/ps.hpp"
#include "shaders/vs.hpp"

namespace sovren {
    void MeshNode3D::init() {
        buffer_ = device()->createBuffer({
            .data = std::as_bytes(mesh_.vertices()),
            .gpuAccess = rhi::AccessModes::Read,
            .bind = rhi::BindModes::VertexBuffer,
        });
        pipeline_ = device()->createPipeline({
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

        SceneNode3D::init();
    }
}

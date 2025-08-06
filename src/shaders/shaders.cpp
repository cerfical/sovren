#include "rhi/AccessModes.hpp"
#include "rhi/BindModes.hpp"
#include "rhi/Format.hpp"
#include "rhi/ShaderLang.hpp"
#include "rhi/VertexAttr.hpp"

#include "MeshNode3D.hpp"

#include <span>
#include <vector>

// TODO: Implement a better way to load shaders
using BYTE = unsigned char;

#include "shaders/ps.hpp"
#include "shaders/vs.hpp"

namespace sovren {
    void MeshNode3D::onRender() {
        if(!buffer_) {
            buffer_ = device()->createBuffer({
                .data = std::as_bytes(mesh_.vertices()),
                .gpuAccess = rhi::AccessModes::Read,
                .bind = rhi::BindModes::VertexBuffer,
            });
        }

        if(!pipeline_) {
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
        }

        context3D()->setPipeline(pipeline_.get());
        context3D()->setVertexBuffer(buffer_.get(), 12);
        context3D()->draw(mesh_.vertices().size());
    }
}

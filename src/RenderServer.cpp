#include <sovren/log/Log.hpp>

#include <sovren/math/Mat4x4.hpp>

#include <sovren/pal/EventPump.hpp>

#include <sovren/rhi/AccessModes.hpp>
#include <sovren/rhi/BindModes.hpp>
#include <sovren/rhi/Device.hpp>
#include <sovren/rhi/Format.hpp>
#include <sovren/rhi/ShaderLang.hpp>
#include <sovren/rhi/VertexAttr.hpp>

#include <sovren/RenderServer.hpp>

#include <cstddef>
#include <memory>
#include <vector>

// TODO: Implement a better way to load shaders
using BYTE = unsigned char;

#include "shaders/ps.hpp"
#include "shaders/vs.hpp"

namespace sovren {
    RenderServer::RenderServer(log::Log log)
        : log_(std::move(log))
        , palLog_(log_.withTags("PAL"))
        , eventPump_(pal::EventPump::create(palLog_))
        , device_(rhi::Device::create())
        , context3D_(device_->createContext3D())
        , context2D_(device_->createContext2D()) {

        objectBuffer_ = device_->createBuffer({
            .size = sizeof(Mat4x4),
            .cpuAccess = rhi::AccessModes::Write,
            .gpuAccess = rhi::AccessModes::Read,
            .bind = rhi::BindModes::ConstantBuffer,
        });

        pipeline_ = device_->createPipeline({
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

        log_.info("Render server initialized");
    }
}

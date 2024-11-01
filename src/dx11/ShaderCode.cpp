#include "pal/rhi/dx11/ShaderCode.hpp"

namespace sovren::rhi::dx11 {
    using BYTE = unsigned char;

    namespace {
#include "shaders/ps.hpp"
#include "shaders/vs.hpp"
    }

    auto ShaderCode::vertexShader() noexcept -> std::span<const std::byte> {
        return std::as_bytes(std::span(vs));
    }

    auto ShaderCode::pixelShader() noexcept -> std::span<const std::byte> {
        return std::as_bytes(std::span(ps));
    }
}

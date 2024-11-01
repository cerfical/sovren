#pragma once

#include <cstddef>
#include <span>

namespace sovren::rhi::dx11 {

    class ShaderCode {
    public:

        [[nodiscard]]
        static auto vertexShader() noexcept -> std::span<const std::byte>;


        [[nodiscard]]
        static auto pixelShader() noexcept -> std::span<const std::byte>;


        ShaderCode() = delete;
    };

}

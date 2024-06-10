#pragma once

#include <cstddef>
#include <span>

namespace reni::rhi::dx {

    class ShaderCode {
    public:

        static std::span<const std::byte> vertexShader() noexcept;

        static std::span<const std::byte> pixelShader() noexcept;


        ShaderCode(const ShaderCode&) = delete;
        ShaderCode& operator=(const ShaderCode&) = delete;

        ShaderCode(ShaderCode&&) = delete;
        ShaderCode& operator=(ShaderCode&&) = delete;

        ShaderCode() = delete;
        ~ShaderCode() = delete;
    };

}

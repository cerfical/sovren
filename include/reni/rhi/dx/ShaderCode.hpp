#pragma once

#include "../../util/NonCopyable.hpp"
#include "../../util/NonMovable.hpp"

#include <span>

namespace reni::rhi::dx {

	class ShaderCode : private NonCopyable, private NonMovable {
	public:

		static std::span<const std::byte> vertexShader() noexcept;
		
		static std::span<const std::byte> pixelShader() noexcept;

	private:
		ShaderCode() = delete;
		~ShaderCode() = delete;
	};

}
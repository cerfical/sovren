#include "rhi/dx/ShaderCode.hpp"

namespace reni::rhi::dx {
	using BYTE = unsigned char;
	
	namespace {
		#include "shaders/ps.hpp"
		#include "shaders/vs.hpp"
	}

	std::span<const std::byte> ShaderCode::vertexShader() noexcept {
		return std::as_bytes(std::span(vs));
	}	
	
	std::span<const std::byte> ShaderCode::pixelShader() noexcept {
		return std::as_bytes(std::span(ps));
	}
}
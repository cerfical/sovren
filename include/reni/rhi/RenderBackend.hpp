#pragma once

#include "../util/types.hpp"

#include "SwapChain.hpp"
#include "RenderContext.hpp"
#include "VertexBuffer.hpp"

#include <memory>
#include <span>

namespace reni::rhi {

	class RenderBackend : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderBackend() = default;


		virtual std::unique_ptr<SwapChain> createSwapChain(void* window) = 0;
		
		virtual std::unique_ptr<RenderContext> createRenderContext() = 0;
		
		virtual std::unique_ptr<VertexBuffer> createVertexBuffer(std::span<const std::byte> data) = 0;

	};

}
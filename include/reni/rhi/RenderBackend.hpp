#pragma once

#include "../util/types.hpp"

#include "SwapChain.hpp"
#include "CommandList.hpp"
#include "RenderContext.hpp"

#include <memory>

namespace reni::rhi {

	class RenderBackend : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderBackend() = default;


		virtual std::unique_ptr<SwapChain> createSwapChain(void* window) = 0;
		
		virtual std::unique_ptr<CommandList> createCommandList() = 0;

		virtual std::unique_ptr<RenderContext> createRenderContext() = 0;

	};

}
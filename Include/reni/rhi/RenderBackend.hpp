#ifndef RENI_RHI_RENDER_BACKEND_HEADER
#define RENI_RHI_RENDER_BACKEND_HEADER

#include "../utils.hpp"

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

#endif
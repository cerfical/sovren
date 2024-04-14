#ifndef RENI_PAL_RENDER_BACKEND_HEADER
#define RENI_PAL_RENDER_BACKEND_HEADER

#include "../utils.hpp"

#include "Render.hpp"
#include "SwapChain.hpp"

#include <memory>

namespace reni::pal {

	class Window;

	
	class RenderBackend : private NonCopyable, private NonMovable {
	public:

		virtual ~RenderBackend() = default;


		std::unique_ptr<SwapChain> createSwapChain(Window& window);

		std::unique_ptr<Render> createRender(RenderType type);


	protected:

		virtual std::unique_ptr<SwapChain> createSwapChainFromWindowHandle(void* windowHandle) = 0;
		
		virtual std::unique_ptr<Render> createRender2d() = 0;

	};

}

#endif
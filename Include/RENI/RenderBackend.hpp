#ifndef RENI_RENDER_BACKEND_HEADER
#define RENI_RENDER_BACKEND_HEADER

#include "util_types.hpp"
#include <memory>

namespace RENI {

	class Render;
	class SwapChain;
	class Window;



	enum class RenderType {
		Render2D
	};



	/**
	 * @brief Abstracts away different rendering APIs.
	*/
	class RenderBackend : private NonCopyable, private NonMovable {
	public:

		static RenderBackend* get();


		virtual ~RenderBackend() = default;


		std::unique_ptr<SwapChain> createSwapChain(Window& window);

		std::unique_ptr<Render> createRender(RenderType type);


	protected:

		virtual std::unique_ptr<SwapChain> createSwapChainFromWindowHandle(void* windowHandle) = 0;
		
		virtual std::unique_ptr<Render> createRender2d() = 0;

	};

}

#endif
#include "RenderBackend.hpp"

#include "DirectX/DxRenderBackend.hpp"
#include "Window.hpp"

namespace RENI {
	RenderBackend* RenderBackend::get() {
		static DxRenderBackend instance;
		return &instance;
	}


	std::unique_ptr<SwapChain> RenderBackend::createSwapChain(Window& window) {
		return createSwapChainFromWindowHandle(window.nativeHandle());
	}


	std::unique_ptr<Render> RenderBackend::createRender(RenderType type) {
		switch(type) {
			case RenderType::Render2D: {
				return createRender2d();
			}
			default: {
				return nullptr;
			}
		}
	}
}
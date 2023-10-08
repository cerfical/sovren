#include "DxRenderEngine.hpp"
#include "DxWindowDevice.hpp"
#include "Window.hpp"

namespace RENI {
	std::unique_ptr<RenderDevice> DxRenderEngine::createWindowDevice(Window* window) {
		return std::make_unique<DxWindowDevice>(
			reinterpret_cast<HWND>(window->nativeHandle())
		);
	}
}
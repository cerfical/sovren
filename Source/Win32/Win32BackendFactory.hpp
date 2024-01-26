#ifndef RENI_WIN32_BACKEND_FACTORY_HEADER
#define RENI_WIN32_BACKEND_FACTORY_HEADER

#include "../DirectX/DxRenderBackend.hpp"
#include "BackendFactory.hpp"

namespace RENI {

	class Win32BackendFactory : public BackendFactory {
	public:

		std::unique_ptr<RenderBackend> createRender(void* window) override {
			return std::make_unique<DxRenderBackend>(static_cast<HWND>(window));
		}

	};

}

#endif
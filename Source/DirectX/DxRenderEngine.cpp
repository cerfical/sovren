#include "DxRenderEngine.hpp"
#include "DxWindowDevice.hpp"
#include "Window.hpp"

namespace {
	constexpr auto deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
		| D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support

#if not defined(NDEBUG) || defined(_DEBUG)
		| D3D11_CREATE_DEVICE_DEBUG
#endif
	;
}

namespace RENI {
	DxRenderEngine::DxRenderEngine() {
		safeComApiCall(D3D11CreateDevice,
			nullptr, // use default IDXGIAdapter
			D3D_DRIVER_TYPE_HARDWARE, // a hardware driver
			nullptr, // no software rasterizer
			deviceFlags,
			nullptr, 0, // default feature levels
			D3D11_SDK_VERSION,
			&m_d3dDevice,
			nullptr,
			&m_d3dContext
		);
	}

	std::unique_ptr<RenderDevice> DxRenderEngine::createWindowDevice(Window* window) {
		return std::make_unique<DxWindowDevice>(
			m_d3dDevice, reinterpret_cast<HWND>(window->nativeHandle())
		);
	}
}
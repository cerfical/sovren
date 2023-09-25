#include "D3DRender.hpp"
#include "Window.hpp"

namespace {
#define RENI_D3D11_COMMON_DEVICE_FLAGS D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT

#ifdef NDEBUG
#define RENI_D3D11_DEVICE_FLAGS RENI_D3D11_COMMON_DEVICE_FLAGS
#else
#define RENI_D3D11_DEVICE_FLAGS RENI_D3D11_COMMON_DEVICE_FLAGS | D3D11_CREATE_DEVICE_DEBUG
#endif

	constexpr auto bufferCount = 2;
}

namespace RENI {
	void D3DRender::createDrawRt() {
		ComPtr<IDXGISurface> surface;
		safeComApiCall(&IDXGISwapChain::GetBuffer, m_swapChain, 0, IID_PPV_ARGS(&surface));
		safeComApiCall([this, &surface]() {
			return m_d2dFactory->CreateDxgiSurfaceRenderTarget(
				surface, D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE)
				), &m_drawRt
			);
		});
	}

	void D3DRender::resizeBuffers(const Size2D& s) {
		m_drawRt.Release();

		safeComApiCall(&IDXGISwapChain::ResizeBuffers, m_swapChain,
			bufferCount,
			gsl::narrow_cast<UINT>(s.width()),
			gsl::narrow_cast<UINT>(s.height()),
			DXGI_FORMAT_UNKNOWN,
			0
		);

		createDrawRt();
	}


	D3DRender::D3DRender(Window& window)
		: m_window(&window), m_bufferSize(1, 1) {
		// initialize swap chain
		const DXGI_SWAP_CHAIN_DESC scDesc {
			gsl::narrow_cast<UINT>(m_bufferSize.width()), // width
			gsl::narrow_cast<UINT>(m_bufferSize.height()), // height
			1, 60, // refresh rate
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING_UNSPECIFIED,
			1, 0, // sample description
			DXGI_USAGE_RENDER_TARGET_OUTPUT,
			bufferCount,
			static_cast<HWND>(window.nativeHandle()),
			TRUE, // windowed mode
			DXGI_SWAP_EFFECT_FLIP_DISCARD,
			0 // flags
		};

		// create Direct3D device resources
		safeComApiCall(D3D11CreateDeviceAndSwapChain,
			nullptr, // use default adapter
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr, // no software module
			RENI_D3D11_DEVICE_FLAGS,
			nullptr, // default feature levels
			0, // default feature levels
			D3D11_SDK_VERSION,
			&scDesc,
			&m_swapChain,
			&m_device,
			nullptr, // supported feature level
			&m_context
		);

		safeComApiCall([this]() { return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory); });

		createDrawRt();

		safeComApiCall([this]() { return m_drawRt->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), &m_drawBrush);
		});
	}


	void D3DRender::startRender() {
		if(const auto windowSize = m_window->size(); m_bufferSize != windowSize) {
			resizeBuffers(windowSize);
			m_bufferSize = windowSize;
		}
		m_drawRt->BeginDraw();
	}

	void D3DRender::endRender() {
		safeComApiCall(&ID2D1RenderTarget::EndDraw, m_drawRt, nullptr, nullptr);
		safeComApiCall(&IDXGISwapChain::Present, m_swapChain, 0, 0);
	}


	void D3DRender::setDrawColor(Color c) {
		m_drawBrush->SetColor(makeColorF(c));
	}

	void D3DRender::clear(Color c) {
		m_drawRt->Clear(makeColorF(c));
	}


	void D3DRender::drawLine(const Line2D& l) {
		m_drawRt->DrawLine(makePoint2F(l.start()), makePoint2F(l.end()), m_drawBrush);
	}

	void D3DRender::drawRect(const Rect2D& r) {
		m_drawRt->DrawRectangle(makeRectF(r), m_drawBrush);
	}

	void D3DRender::fillRect(const Rect2D& r) {
		m_drawRt->FillRectangle(makeRectF(r), m_drawBrush);
	}
}
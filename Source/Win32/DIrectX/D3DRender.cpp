#include "D3DRender.hpp"

#define RENI_D3D11_COMMON_DEVICE_FLAGS D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT

#ifdef NDEBUG
#define RENI_D3D11_DEVICE_FLAGS RENI_D3D11_COMMON_DEVICE_FLAGS
#else
#define RENI_D3D11_DEVICE_FLAGS RENI_D3D11_COMMON_DEVICE_FLAGS | D3D11_CREATE_DEVICE_DEBUG
#endif

namespace RENI {
	D3DRender::D3DRender(const WinWindow& window) {
		DXGI_SWAP_CHAIN_DESC scDesc {
			1, 1, { 1, 60 }, DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED, { 1, 0 },
			DXGI_USAGE_RENDER_TARGET_OUTPUT, buffersCount, window.handle(), TRUE, DXGI_SWAP_EFFECT_FLIP_DISCARD, 0
		};

		safeComApiCall(D3D11CreateDeviceAndSwapChain,
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
			RENI_D3D11_DEVICE_FLAGS, nullptr, 0,
			D3D11_SDK_VERSION, &scDesc, &m_swapChain, &m_device, nullptr, &m_context
		);

		safeComApiCall([this]() { return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory); });
		resizeBuffers(window.clientSize());
		safeComApiCall([this]() { return m_windowRt->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), &m_drawBrush);
		});
	}


	void D3DRender::start() {
		m_windowRt->BeginDraw();
	}

	void D3DRender::end() {
		m_windowRt->EndDraw();
		m_swapChain->Present(0, 0);
	}


	void D3DRender::drawLine(const Line2D& line) {
		m_windowRt->DrawLine(makePoint2F(line.start()), makePoint2F(line.end()), m_drawBrush);
	}

	void D3DRender::drawRect(const Rect2D& rect) {
		m_windowRt->DrawRectangle(makeRectF(rect), m_drawBrush);
	}
	void D3DRender::fillRect(const Rect2D& rect) {
		m_windowRt->FillRectangle(makeRectF(rect), m_drawBrush);
	}

	void D3DRender::setDrawColor(Color color) {
		m_drawBrush->SetColor(makeColorF(color));
	}

	void D3DRender::clear(Color color) {
		m_windowRt->Clear(makeColorF(color));
	}


	void D3DRender::resizeBuffers(const Size2D& size) {
		m_windowRt.Release();
		safeComApiCall([this, &size]() {
			return m_swapChain->ResizeBuffers(
				buffersCount, gsl::narrow_cast<UINT>(size.width()), gsl::narrow_cast<UINT>(size.height()), DXGI_FORMAT_UNKNOWN, 0
			);
		});

		ComPtr<IDXGISurface> surface;
		safeComApiCall([this, &surface]() { return m_swapChain->GetBuffer(0, IID_PPV_ARGS(&surface)); });

		safeComApiCall([this, &surface]() {
			return m_d2dFactory->CreateDxgiSurfaceRenderTarget(
				surface, D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE)
				), &m_windowRt
			);
		});
	}
}
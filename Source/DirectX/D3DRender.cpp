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
	void D3DRender::CreateDrawRt() {
		ComPtr<IDXGISurface> surface;
		SafeComApiCall(&IDXGISwapChain::GetBuffer, m_swapChain, 0, IID_PPV_ARGS(&surface));
		SafeComApiCall([this, &surface]() {
			return m_d2dFactory->CreateDxgiSurfaceRenderTarget(
				surface, D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE)
				), &m_drawRt
			);
		});
	}

	void D3DRender::ResizeBuffers(const Size2D& s) {
		m_drawRt.Release();

		SafeComApiCall(&IDXGISwapChain::ResizeBuffers, m_swapChain,
			bufferCount,
			gsl::narrow_cast<UINT>(s.GetWidth()),
			gsl::narrow_cast<UINT>(s.GetHeight()),
			DXGI_FORMAT_UNKNOWN,
			0
		);

		CreateDrawRt();
	}


	D3DRender::D3DRender(Window& window)
		: m_window(&window), m_bufferSize(1, 1) {
		// initialize swap chain
		const DXGI_SWAP_CHAIN_DESC scDesc {
			gsl::narrow_cast<UINT>(m_bufferSize.GetWidth()), // width
			gsl::narrow_cast<UINT>(m_bufferSize.GetHeight()), // height
			1, 60, // refresh rate
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING_UNSPECIFIED,
			1, 0, // sample description
			DXGI_USAGE_RENDER_TARGET_OUTPUT,
			bufferCount,
			static_cast<HWND>(window.GetNativeHandle()),
			TRUE, // windowed mode
			DXGI_SWAP_EFFECT_FLIP_DISCARD,
			0 // flags
		};

		// create Direct3D device resources
		SafeComApiCall(D3D11CreateDeviceAndSwapChain,
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

		SafeComApiCall([this]() { return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory); });

		CreateDrawRt();

		SafeComApiCall([this]() { return m_drawRt->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::Black), &m_drawBrush);
		});
	}


	void D3DRender::StartRender() {
		if(const auto windowSize = m_window->GetSize(); m_bufferSize != windowSize) {
			ResizeBuffers(windowSize);
			m_bufferSize = windowSize;
		}
		m_drawRt->BeginDraw();
	}

	void D3DRender::EndRender() {
		SafeComApiCall(&ID2D1RenderTarget::EndDraw, m_drawRt, nullptr, nullptr);
		SafeComApiCall(&IDXGISwapChain::Present, m_swapChain, 0, 0);
	}


	void D3DRender::SetDrawColor(Color c) {
		m_drawBrush->SetColor(MakeColorF(c));
	}

	void D3DRender::Clear(Color c) {
		m_drawRt->Clear(MakeColorF(c));
	}


	void D3DRender::DrawLine(const Line2D& l) {
		m_drawRt->DrawLine(MakePoint2F(l.GetStart()), MakePoint2F(l.GetEnd()), m_drawBrush);
	}

	void D3DRender::DrawRect(const Rect2D& r) {
		m_drawRt->DrawRectangle(MakeRectF(r), m_drawBrush);
	}

	void D3DRender::FillRect(const Rect2D& r) {
		m_drawRt->FillRectangle(MakeRectF(r), m_drawBrush);
	}
}
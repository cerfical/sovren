#include "DxWindowDevice.hpp"
#include <gsl/util>

namespace {
	constexpr auto bufferCount = 2;

	constexpr auto d3dDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
		| D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support
#if not defined(NDEBUG) || defined(_DEBUG)
		| D3D11_CREATE_DEVICE_DEBUG
#endif
	;

	constexpr auto d2dFactoryOptions = D2D1_FACTORY_OPTIONS {
#if not defined(NDEBUG) || defined(_DEBUG)
		D2D1_DEBUG_LEVEL_INFORMATION
#else
		D2D1_DEBUG_LEVEL_NONE
#endif
	};
}

namespace RENI {
	void DxWindowDevice::createD2dRt() {
		ComPtr<IDXGISurface> surface;
		safeComApiCall(&IDXGISwapChain::GetBuffer, m_swapChain, 0, IID_PPV_ARGS(&surface));
		safeComApiCall([this, &surface]() {
			return m_d2dFactory->CreateDxgiSurfaceRenderTarget(
				surface, D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE)
				), &m_d2dRt
			);
		});
	}

	void DxWindowDevice::preDraw() {
		if(!m_drawStarted) {
			m_d2dRt->BeginDraw();
			m_drawStarted = true;
		}
	}


	DxWindowDevice::DxWindowDevice(HWND window) {
		safeComApiCall(D3D11CreateDevice,
			nullptr, // use default IDXGIAdapter
			D3D_DRIVER_TYPE_HARDWARE, // hardware driver
			nullptr, // no software rasterizer
			d3dDeviceFlags,
			nullptr, 0, // default feature levels
			D3D11_SDK_VERSION,
			&m_d3dDevice,
			nullptr, // no need to determine the available feature level
			&m_d3dContext
		);

		// retrieve the IDXGIFactory2 interface associated with the ID3D11Device
		ComPtr<IDXGIDevice> dxgiDevice;
		safeComApiCall([&, this]() {
			return m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		});

		ComPtr<IDXGIAdapter> dxgiAdapter;
		safeComApiCall(&IDXGIDevice::GetAdapter, dxgiDevice, &dxgiAdapter);

		ComPtr<IDXGIFactory2> dxgiFactory;
		safeComApiCall(&IDXGIAdapter::GetParent, dxgiAdapter, IID_PPV_ARGS(&dxgiFactory));

		// initialize and create a swap chain
		DXGI_SWAP_CHAIN_DESC1 scDesc {
			0, 0, // make the buffers size be equal to the window size
			DXGI_FORMAT_R8G8B8A8_UNORM,
			FALSE, // no stereo mode
			1, 0, // sample description
			DXGI_USAGE_RENDER_TARGET_OUTPUT,
			bufferCount,
			DXGI_SCALING_NONE,
			DXGI_SWAP_EFFECT_FLIP_DISCARD,
			DXGI_ALPHA_MODE_UNSPECIFIED,
			0 // no flags
		};

		safeComApiCall(&IDXGIFactory2::CreateSwapChainForHwnd, dxgiFactory,
			m_d3dDevice,
			window, // output window
			&scDesc,
			nullptr, // no fullscreen description
			nullptr, // not restricted to some IDXGIOutput
			&m_swapChain
		);

		// Direct2D resources
		safeComApiCall([this]() {
			return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactoryOptions, &m_d2dFactory);
		});
		createD2dRt();

		safeComApiCall([this]() {
			return m_d2dRt->CreateSolidColorBrush(makeColorF(0, 0, 0, 255), &m_d2dBrush);
		});
	}


	void DxWindowDevice::setSize(const Size2D& newSize) {
		m_d2dRt = nullptr;
		safeComApiCall(&IDXGISwapChain::ResizeBuffers, m_swapChain,
			0, // no change to buffer count
			gsl::narrow_cast<UINT>(newSize.width()),
			gsl::narrow_cast<UINT>(newSize.height()),
			DXGI_FORMAT_UNKNOWN, // no change to buffer format
			0 // no flags
		);
		createD2dRt();
	}

	void DxWindowDevice::presentContent() {
		if(m_drawStarted) {
			safeComApiCall(&ID2D1RenderTarget::EndDraw, m_d2dRt, nullptr, nullptr);
			m_drawStarted = false;
		}
		safeComApiCall(&IDXGISwapChain::Present, m_swapChain, 0, 0);
		m_d2dBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	}


	void DxWindowDevice::drawLine(const Line2D& l) {
		preDraw();
		m_d2dRt->DrawLine(makePoint2F(l.start()), makePoint2F(l.end()), m_d2dBrush);
	}

	void DxWindowDevice::drawRect(const Rect2D& r) {
		preDraw();
		m_d2dRt->DrawRectangle(makeRectF(r), m_d2dBrush);
	}

	void DxWindowDevice::fillRect(const Rect2D& r) {
		preDraw();
		m_d2dRt->FillRectangle(makeRectF(r), m_d2dBrush);
	}


	void DxWindowDevice::setDrawColor(Color c) {
		m_d2dBrush->SetColor(makeColorF(c));
	}

	void DxWindowDevice::clear(Color c) {
		preDraw();
		m_d2dRt->Clear(makeColorF(c));
	}
}
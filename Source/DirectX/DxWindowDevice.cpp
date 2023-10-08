#include "DxWindowDevice.hpp"
#include <gsl/util>

namespace {
	constexpr auto bufferCount = 2;
	constexpr auto bufferSize = 1;
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


	DxWindowDevice::DxWindowDevice(ComPtr<ID3D11Device> device, HWND window) {
		// retrieve the IDXGIFactory associated with the ID3D11Device
		ComPtr<IDXGIDevice> dxgiDevice;
		safeComApiCall([&]() {
			return device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		});

		ComPtr<IDXGIAdapter> dxgiAdapter;
		safeComApiCall(&IDXGIDevice::GetAdapter, dxgiDevice, &dxgiAdapter);

		ComPtr<IDXGIFactory> dxgiFactory;
		safeComApiCall(&IDXGIAdapter::GetParent, dxgiAdapter, IID_PPV_ARGS(&dxgiFactory));

		// initialize and create a new swap chain
		DXGI_SWAP_CHAIN_DESC scDesc {
			0, 0, // make the buffers size be equal to the window size
			1, 60, // refresh rate
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			DXGI_MODE_SCALING_UNSPECIFIED,
			1, 0, // sample description
			DXGI_USAGE_RENDER_TARGET_OUTPUT,
			bufferCount,
			window,
			TRUE, // windowed mode
			DXGI_SWAP_EFFECT_FLIP_DISCARD,
			0 // flags
		};
		safeComApiCall(&IDXGIFactory::CreateSwapChain, dxgiFactory, device, &scDesc, &m_swapChain);

		// Direct2D resources
		safeComApiCall([this]() {
			return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_d2dFactory);
		});
		createD2dRt();

		safeComApiCall([this]() {
			return m_d2dRt->CreateSolidColorBrush(makeColorF(0, 0, 0, 255), &m_d2dBrush);
		});
	}


	void DxWindowDevice::startDraw() {
		if(!m_drawStarted) {
			m_d2dRt->BeginDraw();
			m_drawStarted = true;
		}
	}
	
	void DxWindowDevice::endDraw() {
		if(m_drawStarted) {
			safeComApiCall(&ID2D1RenderTarget::EndDraw, m_d2dRt, nullptr, nullptr);
			safeComApiCall(&IDXGISwapChain::Present, m_swapChain, 0, 0);
			m_drawStarted = false;
		}
	}


	void DxWindowDevice::resize(const Size2D& newSize) {
		m_d2dRt = nullptr;
		safeComApiCall(&IDXGISwapChain::ResizeBuffers, m_swapChain,
			bufferCount,
			gsl::narrow_cast<UINT>(newSize.width()),
			gsl::narrow_cast<UINT>(newSize.height()),
			DXGI_FORMAT_UNKNOWN,
			0 // flags
		);
		createD2dRt();
	}

	void DxWindowDevice::setDrawColor(Color c) {
		m_d2dBrush->SetColor(makeColorF(c));
	}


	void DxWindowDevice::drawLine(const Line2D& l) {
		if(m_drawStarted) {
			m_d2dRt->DrawLine(makePoint2F(l.start()), makePoint2F(l.end()), m_d2dBrush);
		}
	}

	void DxWindowDevice::drawRect(const Rect2D& r) {
		if(m_drawStarted) {
			m_d2dRt->DrawRectangle(makeRectF(r), m_d2dBrush);
		}
	}

	void DxWindowDevice::fillRect(const Rect2D& r) {
		if(m_drawStarted) {
			m_d2dRt->FillRectangle(makeRectF(r), m_d2dBrush);
		}
	}

	void DxWindowDevice::clear(Color c) {
		if(m_drawStarted) {
			m_d2dRt->Clear(makeColorF(c));
		}
	}
}
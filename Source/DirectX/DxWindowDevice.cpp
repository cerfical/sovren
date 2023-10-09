#include "DxWindowDevice.hpp"
#include <gsl/util>


namespace {
	constexpr auto bufferCount = 2;
	constexpr auto bufferMinSize = 1;
	const auto drawColor = D2D1::ColorF(D2D1::ColorF::Black);

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
	void DxWindowDevice::d2dCreateRt() {
		// obtain the swap chain's back buffer and create a Direct2D render target onto it
		ComPtr<IDXGISurface> surface;
		comCheck(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&surface)));
		comCheck(m_d2dFactory->CreateDxgiSurfaceRenderTarget(
			surface, D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_HARDWARE,
				D2D1::PixelFormat(
					DXGI_FORMAT_UNKNOWN,
					D2D1_ALPHA_MODE_IGNORE
				)
			), &m_drawRt
		));
	}


	void DxWindowDevice::startDraw() {
		// start the drawing sequence on the Direct2D render target, if necessary
		if(!m_drawStarted) {
			m_drawRt->BeginDraw();
			m_drawStarted = true;
		}
	}

	void DxWindowDevice::endDraw() {
		// end the drawing sequence, if there is one
		if(m_drawStarted) {
			comCheck(m_drawRt->EndDraw());
			m_drawStarted = false;
		}
	}


	DxWindowDevice::DxWindowDevice(HWND window) {
		// allocate Direct3D resources
		comCheck(D3D11CreateDevice(
			NULL, // use default IDXGIAdapter
			D3D_DRIVER_TYPE_HARDWARE, NULL, // hardware driver
			d3dDeviceFlags,
			NULL, 0, // default feature levels
			D3D11_SDK_VERSION,
			&m_d3dDevice,
			NULL, // no need to determine the available feature level
			&m_d3dContext
		));


		// retrieve the IDXGIFactory2 interface associated with the ID3D11Device
		ComPtr<IDXGIDevice> dxgiDevice;
		comCheck(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

		ComPtr<IDXGIAdapter> dxgiAdapter;
		comCheck(dxgiDevice->GetAdapter(&dxgiAdapter));

		ComPtr<IDXGIFactory2> dxgiFactory;
		comCheck(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));


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

		comCheck(dxgiFactory->CreateSwapChainForHwnd(
			m_d3dDevice,
			window, // output window
			&scDesc,
			NULL, // no fullscreen description
			NULL, // not restricted to some IDXGIOutput
			&m_swapChain
		));
		
		// set cached size of the back buffers to its initial value
		comCheck(m_swapChain->GetDesc1(&scDesc));
		m_bufferSize = {
			gsl::narrow_cast<int>(scDesc.Width),
			gsl::narrow_cast<int>(scDesc.Height)
		};


		// allocate Direct2D resources
		comCheck(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactoryOptions, &m_d2dFactory
		));
		d2dCreateRt();

		comCheck(m_drawRt->CreateSolidColorBrush(drawColor, &m_drawBrush));
	}


	void DxWindowDevice::setSize(const Size2D& size) {
		// limit the buffer size to some minimum value
		const auto newSize = Size2D(
			max(size.width(), bufferMinSize),
			max(size.height(), bufferMinSize)
		);

		m_drawRt = nullptr;
		comCheck(m_swapChain->ResizeBuffers(
			0, // preserve the existing number of buffers
			gsl::narrow_cast<UINT>(newSize.width()),
			gsl::narrow_cast<UINT>(newSize.height()),
			DXGI_FORMAT_UNKNOWN, // no change to buffer format
			0 // no flags
		));
		d2dCreateRt();

		m_bufferSize = newSize;
	}

	const Size2D& DxWindowDevice::size() const {
		return m_bufferSize;
	}


	void DxWindowDevice::presentContent() {
		endDraw();
		comCheck(m_swapChain->Present(0, 0));

		// reset the draw color to the default one
		m_drawBrush->SetColor(drawColor);
	}


	void DxWindowDevice::drawLine(const Line2D& line) {
		startDraw();
		m_drawRt->DrawLine(toPoint2F(line.start()), toPoint2F(line.end()), m_drawBrush);
	}

	void DxWindowDevice::drawRect(const Rect2D& rect) {
		startDraw();
		m_drawRt->DrawRectangle(toRectF(rect), m_drawBrush);
	}

	void DxWindowDevice::fillRect(const Rect2D& rect) {
		startDraw();
		m_drawRt->FillRectangle(toRectF(rect), m_drawBrush);
	}


	void DxWindowDevice::setDrawColor(Color col) {
		m_drawBrush->SetColor(toColorF(col));
	}

	void DxWindowDevice::clear(Color col) {
		startDraw();
		m_drawRt->Clear(toColorF(col));
	}
}
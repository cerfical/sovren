#ifndef RENI_DX_RENDER_DEVICE_HEADER
#define RENI_DX_RENDER_DEVICE_HEADER

#include "RenderDevice.hpp"

#include "DxRender2D.hpp"
#include "utils.hpp"

#include <gsl/narrow>
#include <dxgi1_2.h>

namespace RENI {

	class DxRenderDevice : public RenderDevice {
	public:

		explicit DxRenderDevice(HWND window) {
			// allocate Direct3D resources
			comCheck(D3D11CreateDevice(
				NULL, // use default IDXGIAdapter
				D3D_DRIVER_TYPE_HARDWARE, NULL, // hardware driver
				deviceFlags,
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
			DXGI_SWAP_CHAIN_DESC1 scDesc{
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

			comCheck(m_swapChain->GetDesc1(&scDesc));
			m_bufferSize = {
				gsl::narrow_cast<int>(scDesc.Width),
				gsl::narrow_cast<int>(scDesc.Height)
			};
			resetRenderTarget2d();
		}

		DxRenderDevice(const DxRenderDevice&) = delete;
		DxRenderDevice& operator=(const DxRenderDevice&) = delete;

		~DxRenderDevice() override = default;



		void drawScene(const RenderGraph& scene) override {
			m_render2d.startRender();
			for(const auto& n : scene.rootNodes()) {
				m_render2d.render(*n);
			}
			m_render2d.endRender();

			comCheck(m_swapChain->Present(0, 0));
		}



		void setSize(Size2D s) override {
			m_render2d.setRenderTarget(nullptr);
			comCheck(m_swapChain->ResizeBuffers(
				0, // preserve the existing number of buffers
				gsl::narrow_cast<UINT>(s.width),
				gsl::narrow_cast<UINT>(s.height),
				DXGI_FORMAT_UNKNOWN, // no change to buffer format
				0 // no flags
			));
			resetRenderTarget2d();
			m_bufferSize = s;
		}
		
		Size2D size() const override {
			return m_bufferSize;
		}

		void clear(Color c) override {
			m_render2d.clearRenderTarget(c);
		}

		

	private:
		constexpr static auto deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
			| D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support
#if not defined(NDEBUG) || defined(_DEBUG)
			| D3D11_CREATE_DEVICE_DEBUG
#endif
		;
		constexpr static auto bufferCount = 2;


		void resetRenderTarget2d() {
			ComPtr<IDXGISurface> surface;
			comCheck(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&surface)));
			m_render2d.setRenderTarget(surface);
		}


		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
		ComPtr<IDXGISwapChain1> m_swapChain;
		DxRender2D m_render2d;

		Size2D m_bufferSize;
	};

}

#endif
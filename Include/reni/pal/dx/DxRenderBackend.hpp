#ifndef RENI_PAL_DX_RENDER_BACKEND_HEADER
#define RENI_PAL_DX_RENDER_BACKEND_HEADER

#include "../RenderBackend.hpp"

#include "DxRender2D.hpp"
#include "DxSwapChain.hpp"
#include "utils.hpp"

#include <dxgi1_2.h>
#include <d3d11.h>
#include <d2d1.h>

namespace reni::pal::dx {

	class DxRenderBackend : public RenderBackend {
	public:

		DxRenderBackend() {
			static constexpr UINT d3dDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
				| D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support
#if not defined(NDEBUG) || defined(_DEBUG)
				| D3D11_CREATE_DEVICE_DEBUG
#endif
			;

			safeApiCall(D3D11CreateDevice(
				nullptr, // use default IDXGIAdapter
				D3D_DRIVER_TYPE_HARDWARE, nullptr, // hardware driver
				d3dDeviceFlags,
				nullptr, 0, // default feature levels
				D3D11_SDK_VERSION,
				&m_d3dDevice,
				nullptr, // no need to determine the available feature level
				nullptr
			));


			static constexpr D2D1_FACTORY_OPTIONS d2dFactoryOptions = {
	#if not defined(NDEBUG) || defined(_DEBUG)
				D2D1_DEBUG_LEVEL_INFORMATION
	#else
				D2D1_DEBUG_LEVEL_NONE
	#endif
			};

			safeApiCall(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactoryOptions, &m_d2dFactory));
		}

	private:
		std::unique_ptr<SwapChain> createSwapChainFromWindowHandle(void* window) override {
					// retrieve the IDXGIFactory2 interface associated with the ID3D11Device
			ComPtr<IDXGIDevice> dxgiDevice;
			safeApiCall(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

			ComPtr<IDXGIAdapter> dxgiAdapter;
			safeApiCall(dxgiDevice->GetAdapter(&dxgiAdapter));

			ComPtr<IDXGIFactory2> dxgiFactory;
			safeApiCall(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

			// initialize and create a swap chain
			DXGI_SWAP_CHAIN_DESC1 scDesc = {
				0, 0, // make the buffers size be equal to the window size
				DXGI_FORMAT_R8G8B8A8_UNORM,
				FALSE, // no stereo mode
				1, 0, // sample description
				DXGI_USAGE_RENDER_TARGET_OUTPUT,
				2, // buffer count
				DXGI_SCALING_NONE,
				DXGI_SWAP_EFFECT_FLIP_DISCARD,
				DXGI_ALPHA_MODE_UNSPECIFIED,
				0 // no flags
			};

			ComPtr<IDXGISwapChain1> swapChain;
			safeApiCall(dxgiFactory->CreateSwapChainForHwnd(
				m_d3dDevice,
				static_cast<HWND>(window), // output window
				&scDesc,
				nullptr, // no fullscreen description
				nullptr, // not restricted to some IDXGIOutput
				&swapChain
			));

			return std::make_unique<DxSwapChain>(swapChain, m_d2dFactory);
		}
		
		
		std::unique_ptr<Render> createRender2d() override {
			return std::make_unique<DxRender2D>();
		}


		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID2D1Factory> m_d2dFactory;
	};

}

#endif
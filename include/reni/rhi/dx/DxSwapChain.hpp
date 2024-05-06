#pragma once

#include "../SwapChain.hpp"

#include "DxRenderTarget.hpp"
#include "util.hpp"

#include <optional>

#include <dxgi1_2.h>
#include <d2d1_1.h>
#include <d3d11.h>

namespace reni::rhi::dx {

	class DxSwapChain : public SwapChain {
	public:

		DxSwapChain(HWND window, ID3D11Device* d3dDevice, ID2D1DeviceContext* d2dContext)
			: m_d2dContext(d2dContext) {

			// retrieve the IDXGIFactory2 interface associated with the ID3D11Device
			ComPtr<IDXGIDevice> dxgiDevice;
			safeApiCall(d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));

			ComPtr<IDXGIAdapter> dxgiAdapter;
			safeApiCall(dxgiDevice->GetAdapter(&dxgiAdapter));

			ComPtr<IDXGIFactory2> dxgiFactory;
			safeApiCall(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

			// initialize and create a swap chain
			DXGI_SWAP_CHAIN_DESC1 scDesc = {
				0, 0, // make the buffer size be equal to the window size
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
				d3dDevice,
				window, // output window
				&scDesc,
				nullptr, // no fullscreen description
				nullptr, // not restricted to some IDXGIOutput
				&swapChain
			));
			safeApiCall(swapChain->GetDesc1(&scDesc));
	
			m_swapChain = swapChain;
			setBufferSize({
				static_cast<int>(scDesc.Width),
				static_cast<int>(scDesc.Height)
			});
		}


		RenderTarget& frontBuffer() override {
			return m_frontBuffer.value();
		}


		void swapBuffers() override {
			safeApiCall(m_swapChain->Present(0, 0));
		}


		void setBufferSize(Size2 newSize) override {
			m_frontBuffer.reset();
			safeApiCall(m_swapChain->ResizeBuffers(
				0, // preserve the existing number of buffers
				static_cast<UINT>(newSize.width),
				static_cast<UINT>(newSize.height),
				DXGI_FORMAT_UNKNOWN, // no change to buffer format
				0 // no flags
			));

			ComPtr<IDXGISurface> frontBuffer;
			safeApiCall(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&frontBuffer)));
			m_frontBuffer.emplace(frontBuffer, m_d2dContext);

			m_bufferSize = newSize;
		}


		Size2 bufferSize() const override {
			return m_bufferSize;
		}


	private:
		std::optional<DxRenderTarget> m_frontBuffer;
		Size2 m_bufferSize;

		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID2D1DeviceContext> m_d2dContext;
	};

}
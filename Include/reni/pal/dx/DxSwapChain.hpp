#ifndef RENI_PAL_DX_SWAP_CHAIN_HEADER
#define RENI_PAL_DX_SWAP_CHAIN_HEADER

#include "../SwapChain.hpp"

#include "DxRenderTarget.hpp"
#include "utils.hpp"

#include <optional>

#include <d2d1.h>
#include <dxgi.h>

namespace reni::pal::dx {

	class DxSwapChain : public SwapChain {
	public:

		DxSwapChain(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory)
			: m_swapChain(swapChain), m_d2dFactory(d2dFactory) {
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			safeApiCall(swapChain->GetDesc(&swapChainDesc));

			m_bufferSize = {
				static_cast<int>(swapChainDesc.BufferDesc.Width),
				static_cast<int>(swapChainDesc.BufferDesc.Height)
			};
		}


		RenderTarget* frontBuffer() override {
			if(!m_frontBuffer) {
				safeApiCall(m_swapChain->ResizeBuffers(
					0, // preserve the existing number of buffers
					static_cast<UINT>(m_bufferSize.width),
					static_cast<UINT>(m_bufferSize.height),
					DXGI_FORMAT_UNKNOWN, // no change to buffer format
					0 // no flags
				));
				m_frontBuffer.emplace(m_swapChain, m_d2dFactory);
			}
			return &m_frontBuffer.value();
		}


		void swapBuffers() override {
			safeApiCall(m_swapChain->Present(0, 0));
		}


		void setBufferSize(Size2D newSize) override {
			if(m_bufferSize != newSize) {
				m_frontBuffer.reset();
				m_bufferSize = newSize;
			}
		}


		Size2D bufferSize() const override {
			return m_bufferSize;
		}


	private:
		std::optional<DxRenderTarget> m_frontBuffer;
		Size2D m_bufferSize;

		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID2D1Factory> m_d2dFactory;
	};

}

#endif
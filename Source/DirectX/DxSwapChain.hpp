#ifndef RENI_DX_SWAP_CHAIN_HEADER
#define RENI_DX_SWAP_CHAIN_HEADER

#include "SwapChain.hpp"

#include "DxRenderTarget.hpp"
#include "utils.hpp"

#include <optional>
#include <dxgi.h>

namespace reni {

	class DxSwapChain : public SwapChain {
	public:

		DxSwapChain(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory)
			: m_swapChain(swapChain), m_d2dFactory(d2dFactory) {
			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
			comCheck(swapChain->GetDesc(&swapChainDesc));

			m_buffersSize = {
				static_cast<int>(swapChainDesc.BufferDesc.Width),
				static_cast<int>(swapChainDesc.BufferDesc.Height)
			};
		}


		void swapBuffers() override {
			comCheck(m_swapChain->Present(0, 0));
		}


		void setBufferSize(Size2D size) override {
			if(m_buffersSize != size) {
				m_frontBuffer.reset();
				m_buffersSize = size;
			}
		}


		Size2D bufferSize() const override {
			return m_buffersSize;
		}


		RenderTarget& frontBuffer() override {
			if(!m_frontBuffer) {
				comCheck(m_swapChain->ResizeBuffers(
					0, // preserve the existing number of buffers
					static_cast<UINT>(m_buffersSize.width),
					static_cast<UINT>(m_buffersSize.height),
					DXGI_FORMAT_UNKNOWN, // no change to buffer format
					0 // no flags
				));
				m_frontBuffer.emplace(m_swapChain, m_d2dFactory);
			}
			return *m_frontBuffer;
		}


	private:
		std::optional<DxRenderTarget> m_frontBuffer;
		Size2D m_buffersSize;

		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID2D1Factory> m_d2dFactory;
	};

}

#endif
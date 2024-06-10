#pragma once

#include "../../util/Size2.hpp"

#include "../SwapChain.hpp"

#include "DxRenderTarget.hpp"
#include "util.hpp"

#include <d2d1_1.h>
#include <d3d11.h>

#include <optional>

namespace reni::rhi::dx {

	class DxSwapChain : public SwapChain {
	public:

		DxSwapChain(HWND window, ID3D11Device* d3dDevice, ID2D1DeviceContext* d2dContext);


		RenderTarget& frontBuffer() override {
			return m_frontBuffer.value();
		}

		void swapBuffers() override {
			comCheck(m_swapChain->Present(0, 0));
		}


		void setBufferSize(Size2 newSize) override;

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
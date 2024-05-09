#pragma once

#include "../RenderTarget.hpp"
#include "util.hpp"

#include <d3d11.h>
#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderTarget : public RenderTarget {
	public:

		DxRenderTarget(IDXGISurface* renderSurface, ID2D1DeviceContext* d2dContext);


		ID2D1Image* asImage() const noexcept {
			return m_d2dRenderImage;
		}


		ID3D11RenderTargetView* asRenderView() const noexcept {
			return m_d3dRenderView;
		}


		ID3D11DepthStencilView* asDepthView() const noexcept {
			return m_d3dDepthView;
		}


		UINT width() const noexcept {
			return m_width;
		}
		
		
		UINT height() const noexcept {
			return m_height;
		}


	private:
		ComPtr<ID2D1Image> m_d2dRenderImage;
		
		ComPtr<ID3D11RenderTargetView> m_d3dRenderView;
		ComPtr<ID3D11DepthStencilView> m_d3dDepthView;

		UINT m_width = {};
		UINT m_height = {};
	};

}
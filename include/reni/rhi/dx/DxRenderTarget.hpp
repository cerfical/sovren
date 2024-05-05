#ifndef RENI_RHI_DX_RENDER_TARGET_HEADER
#define RENI_RHI_DX_RENDER_TARGET_HEADER

#include "../RenderTarget.hpp"
#include "util.hpp"

#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderTarget : public RenderTarget {
	public:

		DxRenderTarget(IDXGISurface* renderSurface, ID2D1DeviceContext* d2dContext) {
			// allocate Direct2D render target
			safeApiCall(d2dContext->CreateBitmapFromDxgiSurface(renderSurface,
				D2D1::BitmapProperties1(
					D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
					D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
					0.0f,
					0.0f
				),
				&m_bitmap
			));
		}


		void set(ID2D1DeviceContext* d2dContext) {
			d2dContext->SetTarget(m_bitmap);
		}


		void unset(ID2D1DeviceContext* d2dContext) {
			d2dContext->SetTarget(nullptr);
		}


	private:
		ComPtr<ID2D1Bitmap1> m_bitmap;
	};

}

#endif
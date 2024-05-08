#include "rhi/dx/DxRenderTarget.hpp"

namespace reni::rhi::dx {
	DxRenderTarget::DxRenderTarget(IDXGISurface* renderSurface, ID2D1DeviceContext* d2dContext) {
		// allocate Direct2D render target
		ComPtr<ID2D1Bitmap1> d2dRenderBitmap;
		comCheck(d2dContext->CreateBitmapFromDxgiSurface(renderSurface,
			D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE),
				0.0f,
				0.0f
			),
			&d2dRenderBitmap
		));
		m_d2dRenderImage = d2dRenderBitmap;
	}
}
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


		// create Direct3D render target and depth stencil views
		ComPtr<ID3D11Texture2D> renderTexture;
		comCheck(renderSurface->QueryInterface(IID_PPV_ARGS(&renderTexture)));

		ComPtr<ID3D11Device> d3dDevice;
		renderTexture->GetDevice(&d3dDevice);
		comCheck(d3dDevice->CreateRenderTargetView(renderTexture, nullptr, &m_d3dRenderView));

		D3D11_TEXTURE2D_DESC depthDesc = {};
		renderTexture->GetDesc(&depthDesc);
		depthDesc = {
			.Width = depthDesc.Width,
			.Height = depthDesc.Height,
			.MipLevels = 1,
			.ArraySize = 1,
			.Format = DXGI_FORMAT_D32_FLOAT,
			.SampleDesc = { 1, 0 },
			.Usage = D3D11_USAGE_DEFAULT,
			.BindFlags = D3D11_BIND_DEPTH_STENCIL,
			.CPUAccessFlags = 0,
			.MiscFlags = 0
		};

		ComPtr<ID3D11Texture2D> depthTexture;
		comCheck(d3dDevice->CreateTexture2D(&depthDesc, nullptr, &depthTexture));
		comCheck(d3dDevice->CreateDepthStencilView(depthTexture, nullptr, &m_d3dDepthView));

		m_width = depthDesc.Width;
		m_height = depthDesc.Height;
	}
}
#ifndef RENI_PAL_DX_RENDER_TARGET_HEADER
#define RENI_PAL_DX_RENDER_TARGET_HEADER

#include "../RenderTarget.hpp"
#include "utils.hpp"

#include <d3d11.h>
#include <d2d1.h>

namespace reni::pal::dx {

	class DxRenderTarget : public RenderTarget {
	public:

		DxRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) {
			ComPtr<ID3D11Device> d3dDevice;
			safeApiCall(swapChain->GetDevice(IID_PPV_ARGS(&d3dDevice)));
			d3dDevice->GetImmediateContext(&m_d3dContext);


			ComPtr<IDXGISurface> renderTargetSurface;
			safeApiCall(swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetSurface)));

			// allocate Direct2D render target
			safeApiCall(d2dFactory->CreateDxgiSurfaceRenderTarget(
				renderTargetSurface, D2D1::RenderTargetProperties(
					D2D1_RENDER_TARGET_TYPE_HARDWARE,
					D2D1::PixelFormat(
						DXGI_FORMAT_UNKNOWN,
						D2D1_ALPHA_MODE_IGNORE
					)
				), &m_d2dView
			));


			// Direct3D render target and depth buffer
			ComPtr<ID3D11Texture2D> renderTargetTexture;
			safeApiCall(renderTargetSurface->QueryInterface(IID_PPV_ARGS(&renderTargetTexture)));
			safeApiCall(d3dDevice->CreateRenderTargetView(renderTargetTexture, nullptr, &m_d3dView));

			D3D11_TEXTURE2D_DESC depthStencilDesc;
			renderTargetTexture->GetDesc(&depthStencilDesc);
			depthStencilDesc = {
				depthStencilDesc.Width,
				depthStencilDesc.Height,
				1, // mip levels
				1, // array size
				DXGI_FORMAT_D32_FLOAT,
				{ 1, 0 }, // no multisampling
				D3D11_USAGE_DEFAULT,
				D3D11_BIND_DEPTH_STENCIL,
				0, // no cpu access
				0 // no flags
			};

			ComPtr<ID3D11Texture2D> depthStencilTexture;
			safeApiCall(d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilTexture));
			safeApiCall(d3dDevice->CreateDepthStencilView(depthStencilTexture, nullptr, &m_depthStencil));
		}


		void clear(Color clearColor) override {
			const FLOAT colorComponents[] = {
				clearColor.red / 255.0f,
				clearColor.green / 255.0f,
				clearColor.blue / 255.0f,
				clearColor.alpha / 255.0f
			};
			m_d3dContext->ClearRenderTargetView(m_d3dView, colorComponents);
		}


		ID3D11RenderTargetView* d3dView() const {
			return m_d3dView;
		}


		ID2D1RenderTarget* d2dView() const {
			return m_d2dView;
		}


	private:
		ComPtr<ID3D11RenderTargetView> m_d3dView;
		ComPtr<ID3D11DepthStencilView> m_depthStencil;
		ComPtr<ID3D11DeviceContext> m_d3dContext;

		ComPtr<ID2D1RenderTarget> m_d2dView;
	};

}

#endif
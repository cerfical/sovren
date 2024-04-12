#ifndef RENI_DX_RENDER_TARGET_HEADER
#define RENI_DX_RENDER_TARGET_HEADER

#include "RenderTarget.hpp"
#include "utils.hpp"

namespace reni {

	class DxRenderTarget : public RenderTarget {
	public:

		DxRenderTarget(IDXGISwapChain* swapChain, ID2D1Factory* d2dFactory) {
			ComPtr<ID3D11Device> d3dDevice;
			comCheck(swapChain->GetDevice(IID_PPV_ARGS(&d3dDevice)));
			d3dDevice->GetImmediateContext(&m_d3dContext);


			ComPtr<IDXGISurface> renderTargetSurface;
			comCheck(swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargetSurface)));

			// allocate Direct2D render target
			comCheck(d2dFactory->CreateDxgiSurfaceRenderTarget(
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
			comCheck(renderTargetSurface->QueryInterface(IID_PPV_ARGS(&renderTargetTexture)));
			comCheck(d3dDevice->CreateRenderTargetView(renderTargetTexture, nullptr, &m_d3dView));

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
			comCheck(d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilTexture));
			comCheck(d3dDevice->CreateDepthStencilView(depthStencilTexture, nullptr, &m_depthStencil));
		}


		void clear(Color clearColor) override {
			const FLOAT colorComponents[] = {
				static_cast<FLOAT>(clearColor.red),
				static_cast<FLOAT>(clearColor.green),
				static_cast<FLOAT>(clearColor.blue),
				static_cast<FLOAT>(clearColor.alpha)
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
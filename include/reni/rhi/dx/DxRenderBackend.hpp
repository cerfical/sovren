#pragma once

#include "../RenderBackend.hpp"

#include "DxSwapChain.hpp"
#include "DxCommandList.hpp"
#include "DxRenderContext.hpp"

#include "util.hpp"

#include <d3d11.h>
#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderBackend : public RenderBackend {
	public:

		DxRenderBackend() {
			safeApiCall(D3D11CreateDevice(
				nullptr, // use default IDXGIAdapter
				D3D_DRIVER_TYPE_HARDWARE, nullptr, // hardware driver
				d3dDeviceFlags,
				nullptr, 0, // default feature levels
				D3D11_SDK_VERSION,
				&m_d3dDevice,
				nullptr, // no need to determine the available feature level
				nullptr
			));

			safeApiCall(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, d2dFactoryOptions, &m_d2dFactory));


			ComPtr<IDXGIDevice> dxgiDevice;
			safeApiCall(m_d3dDevice->QueryInterface(IID_PPV_ARGS(&dxgiDevice)));
			
			safeApiCall(m_d2dFactory->CreateDevice(dxgiDevice, &m_d2dDevice));
			safeApiCall(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext));
		}


		std::unique_ptr<SwapChain> createSwapChain(void* window) override {
			return std::make_unique<DxSwapChain>(static_cast<HWND>(window), m_d3dDevice, m_d2dContext);
		}


		std::unique_ptr<CommandList> createCommandList() override {
			return std::make_unique<DxCommandList>(m_d2dDevice);
		}


		std::unique_ptr<RenderContext> createRenderContext() override {
			return std::make_unique<DxRenderContext>(m_d2dContext);
		}
		
		
	private:

		static constexpr UINT d3dDeviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED
			| D3D11_CREATE_DEVICE_BGRA_SUPPORT // Direct2D support
#if not defined(NDEBUG) || defined(_DEBUG)
			| D3D11_CREATE_DEVICE_DEBUG
#endif
		;

		static constexpr D2D1_FACTORY_OPTIONS d2dFactoryOptions = {
#if not defined(NDEBUG) || defined(_DEBUG)
			D2D1_DEBUG_LEVEL_INFORMATION
#else
			D2D1_DEBUG_LEVEL_NONE
#endif
		};


		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID2D1Factory1> m_d2dFactory;
		
		ComPtr<ID2D1Device> m_d2dDevice;
		ComPtr<ID2D1DeviceContext> m_d2dContext;
	};

}
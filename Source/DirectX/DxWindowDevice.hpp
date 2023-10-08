#ifndef RENI_DX_WINDOW_DEVICE_HEADER
#define RENI_DX_WINDOW_DEVICE_HEADER

#include "RenderDevice.hpp"
#include "DxUtils.hpp"

#include <dxgi1_2.h>

namespace RENI {
	/**
	 * @brief DirectX render device for a window.
	 */
	class DxWindowDevice : public RenderDevice {
	public:
		/** @{ */
		explicit DxWindowDevice(HWND window);
		~DxWindowDevice() override = default;
		/** @} */

		/** @{ */
		void setSize(const Size2D& newSize) override;
		void presentContent() override;
		/** @} */

		/** @{ */
		void drawLine(const Line2D& l) override;
		void drawRect(const Rect2D& r) override;
		void fillRect(const Rect2D& r) override;
	
		void setDrawColor(Color c) override;
		void clear(Color c) override;
		/** @} */

	private:
		/** @{ */
		void createD2dRt();
		void preDraw();
		/** @} */

		/** @{ */
		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
		ComPtr<IDXGISwapChain1> m_swapChain;

		ComPtr<ID2D1Factory> m_d2dFactory;
		ComPtr<ID2D1SolidColorBrush> m_d2dBrush;
		ComPtr<ID2D1RenderTarget> m_d2dRt;
		/** @} */

		/** @{ */
		bool m_drawStarted = false;
		/** @} */
	};
}

#endif
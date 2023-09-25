#ifndef RENI_DX_WINDOW_DEVICE_HEADER
#define RENI_DX_WINDOW_DEVICE_HEADER

#include "RenderDevice.hpp"
#include "DxUtils.hpp"

namespace RENI {
	/**
	 * @brief DirectX render device for a window.
	 */
	class DxWindowDevice : public RenderDevice {
	public:
		/** @{ */
		DxWindowDevice(ComPtr<ID3D11Device> device, HWND window);
		/** @} */

		/** @{ */
		void startDraw() override;
		void endDraw() override;
		/** @} */

		/** @{ */
		void resize(const Size2D& newSize) override;
		void setDrawColor(Color c) override;
		/** @} */

		/** @{ */
		void drawLine(const Line2D& l) override;
		void drawRect(const Rect2D& r) override;
		void fillRect(const Rect2D& r) override;
	
		void clear(Color c) override;
		/** @} */

	private:
		/** @{ */
		void createD2dRt();
		/** @} */

		/** @{ */
		ComPtr<IDXGISwapChain> m_swapChain;

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
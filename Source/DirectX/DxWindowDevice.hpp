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
		const Size2D& size() const override;

		void presentContent() override;
		/** @} */

		/** @{ */
		void drawLine(const Line2D& line) override;
		void drawRect(const Rect2D& rect) override;
		void fillRect(const Rect2D& rect) override;
	
		void setDrawColor(Color col) override;
		void clear(Color col) override;
		/** @} */

	private:
		/** @{ */
		void d2dCreateRt();

		void startDraw();
		void endDraw();
		/** @} */

		/** @{ */
		ComPtr<ID3D11Device> m_d3dDevice;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
		ComPtr<ID2D1Factory> m_d2dFactory;
		
		ComPtr<IDXGISwapChain1> m_swapChain;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;
		ComPtr<ID2D1RenderTarget> m_drawRt;
		/** @} */

		/** @{ */
		Size2D m_bufferSize;
		bool m_drawStarted = false;
		/** @} */
	};
}

#endif
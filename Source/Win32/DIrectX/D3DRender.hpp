#ifndef RENI_D3D_RENDER_HEADER
#define RENI_D3D_RENDER_HEADER

#include "Render.hpp"

#include "WinWindow.hpp"
#include "DXUtils.hpp"

namespace RENI {
	class D3DRender : public Render {
	public:
		/** @{ */
		explicit D3DRender(const WinWindow& window);
		/** @} */

		/** @{ */
		void start() override;
		void end() override;
		/** @} */

		/** @{ */
		void drawLine(const Line2D& line) override;

		void drawRect(const Rect2D& rect) override;
		void fillRect(const Rect2D& rect) override;

		void setDrawColor(Color color) override;
		void clear(Color color) override;
		/** @} */

		/** @{ */
		void resizeBuffers(const Size2D& size);
		/** @} */

	private:
		constexpr static int buffersCount = 2;

		ComPtr<IDXGISwapChain> m_swapChain;
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;

		ComPtr<ID2D1Factory> m_d2dFactory;
		ComPtr<ID2D1RenderTarget> m_windowRt;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;
	};
}

#endif
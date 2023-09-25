#ifndef RENI_D2D_UI_FACTORY_HEADER
#define RENI_D2D_UI_FACTORY_HEADER

#include "D2DCanvas.hpp"
#include "WinUiFactory.hpp"

namespace RENI {
	/**
	 * @brief Direct2D implementation of WinUiFactory.
	 */
	class D2DUiFactory : public WinUiFactory {
	public:
		/** @{ */
		D2DUiFactory() {
			SafeComApiCall([this]() {
				return D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&m_d2dFactory)
				);
			});
		}
		/** @} */

		/** @{ */
		std::unique_ptr<Canvas> CreateCanvas(WinWindow& window) override {
			ComPtr<ID2D1HwndRenderTarget> renderTarget;
			SafeComApiCall([this, &renderTarget, &window]() {
				return m_d2dFactory->CreateHwndRenderTarget(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(
						window.GetHandle(), MakeSizeU(window.GetClientArea())
					), &renderTarget
				);
			});
			return std::make_unique<D2DCanvas>(renderTarget);
		}
		/** @} */

	private:
		ComPtr<ID2D1Factory> m_d2dFactory;
	};
}

#endif
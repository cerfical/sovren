#ifndef RENI_D2D_UI_FACTORY_HEADER
#define RENI_D2D_UI_FACTORY_HEADER

#include "D2DCanvas.hpp"
#include "D2DUtils.hpp"

#include "../WinUiFactory.hpp"
#include "../WinUtils.hpp"

#include <atlbase.h>
#include <d2d1.h>

namespace RENI::Win32::D2D {
	/**
	 * @brief Direct2D implementation of WinUiFactory.
	 */
	class D2DUiFactory : public WinUiFactory {
	public:
		/** @{ */
		/** @brief Construct a new D2DUiFactory. */
		D2DUiFactory() {
			SafeComApiCall([this]() {
				return D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&d2dFactory)
				);
			});
		}

		/** @brief Destroy the D2DUiFactory. */
		~D2DUiFactory() = default;
		/** @} */

		/** @{ */
		std::unique_ptr<Canvas> MakeCanvas(WinWindow& window) override {
			ATL::CComPtr<ID2D1HwndRenderTarget> renderTarget;
			SafeComApiCall([this, &renderTarget, &window]() {
				return d2dFactory->CreateHwndRenderTarget(
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
		ATL::CComPtr<ID2D1Factory> d2dFactory;
	};
}

#endif
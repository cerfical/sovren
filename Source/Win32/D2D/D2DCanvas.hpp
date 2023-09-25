#ifndef RENI_D2D_CANVAS_HEADER
#define RENI_D2D_CANVAS_HEADER

#include "Canvas.hpp"

#include "../WinUtils.hpp"
#include "D2DUtils.hpp"

#include <atlbase.h>
#include <d2d1.h>

namespace RENI::Win32::D2D {
	/**
	 * @brief Direct2D implementation of DrawingContext.
	 */
	class D2DDrawingContext : public DrawingContext {
	public:
		/** @{ */
		/** @brief Construct a new D2DDrawingContext. */
		explicit D2DDrawingContext(ATL::CComPtr<ID2D1HwndRenderTarget> rt)
		 : renderTarget(rt) {
			SafeComApiCall([this]() {
				return renderTarget->CreateSolidColorBrush(
					D2D1::ColorF(D2D1::ColorF::White), &brush
				);
			});
			renderTarget->BeginDraw();
		}
		
		/** @brief Destroy the D2DDrawingContext. */
		~D2DDrawingContext() {
			SafeComApiCall([this]() { return renderTarget->EndDraw(); });
		}
		/** @} */
		
		/** @{ */
		void DrawRect(const Rect& rect, Color color) override {
			brush->SetColor(MakeColorF(color));
			renderTarget->DrawRectangle(MakeRectF(rect), brush);
		}
		void FillRect(const Rect& rect, Color color) override {
			brush->SetColor(MakeColorF(color));
			renderTarget->FillRectangle(MakeRectF(rect), brush);
		}
		
		void Clear(Color color) override {
			renderTarget->Clear(MakeColorF(color));
		}
		/** @} */

	private:
		ATL::CComPtr<ID2D1HwndRenderTarget> renderTarget;
		ATL::CComPtr<ID2D1SolidColorBrush> brush;
	};



	/**
	 * @brief Direct2D implementation of Canvas.
	 */
	class D2DCanvas : public Canvas {
	public:
		/** @{ */
		/** @brief Construct a new D2DCanvas. */
		explicit D2DCanvas(ATL::CComPtr<ID2D1HwndRenderTarget> rt) noexcept
		 : renderTarget(rt)
		{ }

		/** @brief Destroy the D2DCanvas. */
		~D2DCanvas() = default;
		/** @} */

		/** @{ */
		std::unique_ptr<DrawingContext> BeginDraw() override {
			return std::make_unique<D2DDrawingContext>(renderTarget);
		}
		void Resize(Extent2D size) override {
			SafeComApiCall([this, size]() {
				return renderTarget->Resize(MakeSizeU(size));
			});
		}
		/** @} */

	private:
		ATL::CComPtr<ID2D1HwndRenderTarget> renderTarget;
	};
}

#endif
#ifndef RENI_D2D_CANVAS_HEADER
#define RENI_D2D_CANVAS_HEADER

#include "Canvas.hpp"

#include "WinUtils.hpp"
#include "D2DUtils.hpp"

#include <atlbase.h>
#include <d2d1.h>

namespace RENI::Win32::D2D {
	/**
	 * @brief Direct2D implementation of Canvas.
	 */
	class D2DCanvas : public Canvas {
	public:
		/** @{ */
		/** @brief Construct a new D2DCanvas. */
		explicit D2DCanvas(ATL::CComPtr<ID2D1HwndRenderTarget> rt)
		 : renderTarget(rt) {
			SafeComApiCall([this]() {
				return renderTarget->CreateSolidColorBrush({ }, &brush);
			});
		}
		/** @brief Destroy the D2DCanvas. */
		~D2DCanvas() = default;
		/** @} */

		/** @{ */
		std::unique_ptr<DrawingContext> BeginDraw() override {
			return std::make_unique<D2DDrawingContext>(*this);
		}
		void Resize(const Extent2D& size) override {
			SafeComApiCall([this, size]() {
				return renderTarget->Resize(MakeSizeU(size));
			});
		}
		/** @} */

	private:
		/**
		 * @brief Direct2D implementation of DrawingContext.
		 */
		class D2DDrawingContext : public DrawingContext {
		public:
			/** @{ */
			/** @brief Construct a new D2DDrawingContext. */
			explicit D2DDrawingContext(D2DCanvas& canvas) noexcept
			 : canvas(canvas) {
				canvas.renderTarget->BeginDraw();
			}
			/** @brief Destroy the D2DDrawingContext. */
			~D2DDrawingContext() {
				SafeComApiCall([this]() { return canvas.renderTarget->EndDraw(); });
			}
			/** @} */

			/** @{ */
			void DrawLine(const Line2D& line) override {
				canvas.renderTarget->DrawLine(MakePoint2F(line.start), MakePoint2F(line.end), canvas.brush);
			}

			void DrawRect(const Rect2D& rect) override {
				canvas.renderTarget->DrawRectangle(MakeRectF(rect), canvas.brush);
			}
			void FillRect(const Rect2D& rect) override {
				canvas.renderTarget->FillRectangle(MakeRectF(rect), canvas.brush);
			}

			void SetDrawColor(Color color) override {
				canvas.brush->SetColor(MakeColorF(color));
			}
			void Clear(Color color) override {
				canvas.renderTarget->Clear(MakeColorF(color));
			}
			/** @} */

		private:
			D2DCanvas& canvas;
		};

		ATL::CComPtr<ID2D1HwndRenderTarget> renderTarget;
		ATL::CComPtr<ID2D1SolidColorBrush> brush;
	};
}

#endif
#ifndef RENI_D2D_CANVAS_HEADER
#define RENI_D2D_CANVAS_HEADER

#include "Canvas.hpp"
#include "D2DUtils.hpp"

namespace RENI {
	/**
	 * @brief Direct2D implementation of Canvas.
	 */
	class D2DCanvas : public Canvas {
	public:
		/** @{ */
		explicit D2DCanvas(ComPtr<ID2D1HwndRenderTarget> renderTarget)
			: m_renderTarget(renderTarget) {
			SafeComApiCall([this]() {
				return m_renderTarget->CreateSolidColorBrush({ }, &m_brush);
			});
		}
		/** @} */

		/** @{ */
		std::unique_ptr<DrawingContext> BeginDraw() override {
			return std::make_unique<D2DDrawingContext>(*this);
		}
		void Resize(const Extent2D& size) override {
			SafeComApiCall([this, size]() {
				return m_renderTarget->Resize(MakeSizeU(size));
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
			explicit D2DDrawingContext(D2DCanvas& canvas) noexcept
				: m_canvas(canvas) {
				m_canvas.m_renderTarget->BeginDraw();
			}
		
			~D2DDrawingContext() {
				SafeComApiCall([this]() { return m_canvas.m_renderTarget->EndDraw(); });
			}
			/** @} */

			/** @{ */
			void DrawLine(const Line2D& line) override {
				m_canvas.m_renderTarget->DrawLine(MakePoint2F(line.start), MakePoint2F(line.end), m_canvas.m_brush);
			}

			void DrawRect(const Rect2D& rect) override {
				m_canvas.m_renderTarget->DrawRectangle(MakeRectF(rect), m_canvas.m_brush);
			}
			void FillRect(const Rect2D& rect) override {
				m_canvas.m_renderTarget->FillRectangle(MakeRectF(rect), m_canvas.m_brush);
			}

			void SetDrawColor(Color color) override {
				m_canvas.m_brush->SetColor(MakeColorF(color));
			}
			void Clear(Color color) override {
				m_canvas.m_renderTarget->Clear(MakeColorF(color));
			}
			/** @} */

		private:
			D2DCanvas& m_canvas;
		};

		ComPtr<ID2D1HwndRenderTarget> m_renderTarget;
		ComPtr<ID2D1SolidColorBrush> m_brush;
	};
}

#endif
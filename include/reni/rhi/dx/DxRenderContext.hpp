#pragma once

#include "../RenderContext.hpp"
#include "util.hpp"

#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderContext : public RenderContext {
	public:

		explicit DxRenderContext(ID2D1DeviceContext* d2dContext);
		
		
		void startRender(RenderTarget& rt) override;

		void endRender() override;
		
		
		void drawLine(Point2 start, Point2 end) override {
			m_d2dContext->DrawLine({ start.x, start.y }, { end.x, end.y }, m_drawBrush);
		}


		void drawRect(Point2 topLeft, Point2 botRight) override {
			m_d2dContext->DrawRectangle({ topLeft.x, topLeft.y, botRight.x, botRight.y }, m_drawBrush);
		}


		void clear(Color clearColor) override {
			m_d2dContext->Clear({ clearColor.r, clearColor.g, clearColor.b, clearColor.a });
		}


	private:
		static constexpr auto defaultDrawColor = D2D1::ColorF::Black;

		ComPtr<ID2D1DeviceContext> m_d2dContext;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;
	};

}
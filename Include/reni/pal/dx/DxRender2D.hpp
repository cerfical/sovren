#ifndef RENI_PAL_DX_RENDER_2D_HEADER
#define RENI_PAL_DX_RENDER_2D_HEADER

#include "../../rg.hpp"
#include "../Render.hpp"

#include "DxRenderTarget.hpp"
#include "utils.hpp"

#include <d2d1.h>

namespace reni::pal::dx {

	class DxRender2D : public Render, private rg::NodeVisitor {
	public:

		void renderNode(const rg::RenderNode& n) override {
			n.accept(*this);
		}


		void startRender(RenderTarget& rt) override {
			const auto& dxRt = dynamic_cast<DxRenderTarget&>(rt);
			m_renderTarget = dxRt.d2dView();

			static const D2D1::ColorF defaultDrawColor(D2D1::ColorF::Black);
			if(!m_drawBrush) {
				safeApiCall(m_renderTarget->CreateSolidColorBrush(defaultDrawColor, &m_drawBrush));
			}
			m_renderTarget->BeginDraw();
		}


		void endRender() override {
			safeApiCall(m_renderTarget->EndDraw());
			m_renderTarget = nullptr;
		}


	private:
		void visit(const rg::Line2D& line) override {
			m_renderTarget->DrawLine(makePoint(line.start), makePoint(line.end), m_drawBrush);
		}


		void visit(const rg::Rect2D& rect) override {
			const auto [top, left] = makePoint(rect.topLeft);
			const auto [bottom, right] = makePoint(rect.bottomRight);

			m_renderTarget->DrawRectangle(
				{ left, top, right, bottom }, m_drawBrush
			);
		}


		ComPtr<ID2D1RenderTarget> m_renderTarget;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;
	};

}

#endif
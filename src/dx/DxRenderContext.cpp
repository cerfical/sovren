#include "rhi/dx/DxRenderContext.hpp"
#include "rhi/dx/DxRenderTarget.hpp"

namespace reni::rhi::dx {
	DxRenderContext::DxRenderContext(ID2D1DeviceContext* d2dContext)
		: m_d2dContext(d2dContext) {
		
		comCheck(d2dContext->CreateSolidColorBrush(D2D1::ColorF(defaultDrawColor), &m_drawBrush));
	}


	void DxRenderContext::startRender(RenderTarget& rt) {
		auto& dxRt = dynamic_cast<DxRenderTarget&>(rt);

		m_d2dContext->SetTarget(dxRt.asImage());
		m_d2dContext->BeginDraw();
	}


	void DxRenderContext::endRender() {
		comCheck(m_d2dContext->EndDraw());
		m_d2dContext->SetTarget(nullptr);
	}
}
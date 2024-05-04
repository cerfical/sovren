#ifndef RENI_RHI_DX_RENDER_CONTEXT_HEADER
#define RENI_RHI_DX_RENDER_CONTEXT_HEADER

#include "../RenderContext.hpp"

#include "DxCommandList.hpp"
#include "DxRenderTarget.hpp"

#include "utils.hpp"

#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderContext : public RenderContext {
	public:

		explicit DxRenderContext(ID2D1DeviceContext* d2dContext)
			: m_d2dContext(d2dContext) {}


		void renderCommands(const CommandList& commands, RenderTarget& target) override {
			auto& dxCommands = dynamic_cast<const DxCommandList&>(commands);
			auto& dxTarget = dynamic_cast<DxRenderTarget&>(target);

			dxTarget.set(m_d2dContext);

			m_d2dContext->BeginDraw();
			dxCommands.exec(m_d2dContext);
			safeApiCall(m_d2dContext->EndDraw());

			dxTarget.unset(m_d2dContext);
		}


	private:
		ComPtr<ID2D1DeviceContext> m_d2dContext;
	};

}

#endif
#ifndef RENI_RHI_DX_COMMAND_LIST_HEADER
#define RENI_RHI_DX_COMMAND_LIST_HEADER

#include "../CommandList.hpp"
#include "util.hpp"

#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxCommandList : public CommandList {
	public:

		explicit DxCommandList(ID2D1Device* d2dDevice) {
			safeApiCall(d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_drawContext));
			safeApiCall(m_drawContext->CreateSolidColorBrush(D2D1::ColorF(defaultDrawColor), &m_drawBrush));
		}


		void startRender() override {
			m_drawCommands = nullptr;
			safeApiCall(m_drawContext->CreateCommandList(&m_drawCommands));

			m_drawContext->SetTarget(m_drawCommands);
			m_drawContext->BeginDraw();
		}


		void endRender() override {
			safeApiCall(m_drawContext->EndDraw());
			safeApiCall(m_drawCommands->Close());
		}


		void drawLine(Point2 start, Point2 end) override {
			m_drawContext->DrawLine(makePoint(start), makePoint(end), m_drawBrush);
		}


		void drawRect(Point2 topLeft, Point2 bottomRight) override {
			const auto tl = makePoint(topLeft);
			const auto br = makePoint(bottomRight);
			
			m_drawContext->DrawRectangle(D2D1::RectF(tl.x, tl.y, br.x, br.y), m_drawBrush);
		}


		void clear(Color clearColor) override {
			m_drawContext->Clear(makeColor(clearColor));
		}


		void exec(ID2D1DeviceContext* d2dContext) const {
			d2dContext->DrawImage(m_drawCommands);
		}


	private:
		static constexpr auto defaultDrawColor = D2D1::ColorF::Black;


		ComPtr<ID2D1CommandList> m_drawCommands;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;
		ComPtr<ID2D1DeviceContext> m_drawContext;
	};

}


#endif
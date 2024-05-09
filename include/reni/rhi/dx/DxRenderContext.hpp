#pragma once

#include "../RenderContext.hpp"
#include "util.hpp"

#include <d3d11.h>
#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderContext : public RenderContext {
	public:

		DxRenderContext(ID2D1DeviceContext* d2dContext, ID3D11DeviceContext* d3dContext);
		
		
		void startRender(RenderTarget& rt) override;

		void endRender() override;
		
		
		void drawLine(const Vec2& start, const Vec2& end) override {
			m_d2dContext->DrawLine({ start.x, start.y }, { end.x, end.y }, m_drawBrush);
		}


		void drawRect(const Vec2& topLeft, const Vec2& botRight) override {
			m_d2dContext->DrawRectangle({ topLeft.x, topLeft.y, botRight.x, botRight.y }, m_drawBrush);
		}


		void drawMesh(const VertexBuffer& vert) override;

		
		void setProjection(const Mat4x4& proj) override;


		void clear(Color col) override;


	private:
		static constexpr auto defaultDrawColor = D2D1::ColorF::Black;

		ComPtr<ID2D1DeviceContext> m_d2dContext;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;

		ComPtr<ID3D11Buffer> m_frameBuffer;
		ComPtr<ID3D11DeviceContext> m_d3dContext;
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11PixelShader> m_pixelShader;
	};

}
#pragma once

#include "../RenderContext.hpp"
#include "util.hpp"

#include <optional>

#include <d3d11.h>
#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderContext : public RenderContext {
	public:

		DxRenderContext(ID2D1DeviceContext* d2dContext, ID3D11DeviceContext* d3dContext);
		
		
		void startRender(RenderTarget& rt) override;

		void endRender() override;
		
		
		void drawLine(Vec2 start, Vec2 end) override {
			m_d2dContext->DrawLine({ start.x, start.y }, { end.x, end.y }, m_drawBrush);
		}


		void drawRect(Vec2 topLeft, Vec2 borromRight) override {
			m_d2dContext->DrawRectangle({ topLeft.x, topLeft.y, borromRight.x, borromRight.y }, m_drawBrush);
		}


		void drawMesh(const VertexBuffer& vert) override;

		void clear(Color col) override;


		void setTransformMatrix(const Mat3x3& mat) override;

		void setProjectionMatrix(const Mat4x4& proj) override {
			m_proj = proj;
		}
		
		void setTransformMatrix(const Mat4x4& mat) override {
			m_transform3d = mat;
		}


	private:
		static constexpr auto DefaultDrawColor = D2D1::ColorF::Black;

		void writeCb(ID3D11Buffer* cb, const Mat4x4& mat);

		ComPtr<ID2D1DeviceContext> m_d2dContext;
		ComPtr<ID2D1SolidColorBrush> m_drawBrush;

		ComPtr<ID3D11Buffer> m_frameBuffer;
		ComPtr<ID3D11Buffer> m_objectBuffer;

		ComPtr<ID3D11DeviceContext> m_d3dContext;
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11PixelShader> m_pixelShader;

		std::optional<Mat4x4> m_transform3d;
		std::optional<Mat4x4> m_proj;
	};

}
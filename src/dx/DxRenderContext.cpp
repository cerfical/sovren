#include "rhi/dx/DxRenderContext.hpp"

#include "rhi/dx/DxRenderTarget.hpp"
#include "rhi/dx/DxVertexBuffer.hpp"

#include "rhi/dx/ShaderCode.hpp"

namespace reni::rhi::dx {
	DxRenderContext::DxRenderContext(ID2D1DeviceContext* d2dContext, ID3D11DeviceContext* d3dContext)
		: m_d2dContext(d2dContext), m_d3dContext(d3dContext) {
		
		comCheck(d2dContext->CreateSolidColorBrush(D2D1::ColorF(defaultDrawColor), &m_drawBrush));


		ComPtr<ID3D11Device> d3dDevice;
		d3dContext->GetDevice(&d3dDevice);

		const auto vs = ShaderCode::vertexShader();
		comCheck(d3dDevice->CreateVertexShader(vs.data(), vs.size(), nullptr, &m_vertexShader));

		const auto ps = ShaderCode::pixelShader();
		comCheck(d3dDevice->CreatePixelShader(ps.data(), ps.size(), nullptr, &m_pixelShader));


		const D3D11_BUFFER_DESC cbDesc = {
			.ByteWidth = sizeof(Mat4x4),
			.Usage = D3D11_USAGE_DYNAMIC,
			.BindFlags = D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
			.MiscFlags = 0,
			.StructureByteStride = 0
		};
		comCheck(d3dDevice->CreateBuffer(&cbDesc, nullptr, &m_frameBuffer));
	}


	void DxRenderContext::startRender(RenderTarget& rt) {
		auto& dxrt = dynamic_cast<DxRenderTarget&>(rt);

		m_d2dContext->SetTarget(dxrt.asImage());
		m_d2dContext->BeginDraw();

		m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_d3dContext->VSSetShader(m_vertexShader, nullptr, 0);

		m_d3dContext->VSSetConstantBuffers(0, 1, &m_frameBuffer.p);
		m_d3dContext->PSSetShader(m_pixelShader, nullptr, 0);
		
		const auto rtv = dxrt.asRenderView();
		m_d3dContext->OMSetRenderTargets(1, &rtv, dxrt.asDepthView());

		const D3D11_VIEWPORT viewport = {
			.TopLeftX = 0.0f,
			.TopLeftY = 0.0f,
			.Width = static_cast<FLOAT>(dxrt.width()),
			.Height = static_cast<FLOAT>(dxrt.height()),
			.MinDepth = 0.0f,
			.MaxDepth = 1.0f
		};
		m_d3dContext->RSSetViewports(1, &viewport);
	}


	void DxRenderContext::endRender() {
		comCheck(m_d2dContext->EndDraw());
		m_d2dContext->SetTarget(nullptr);
	}


	void DxRenderContext::drawMesh(const VertexBuffer& vert) {
		const auto& dxvert = dynamic_cast<const DxVertexBuffer&>(vert);

		const auto vb = dxvert.vertexData();
		const UINT stride = dxvert.vertexStride(), offset = 0;

		m_d3dContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_d3dContext->IASetInputLayout(dxvert.inputLayout());

		m_d3dContext->Draw(dxvert.vertexCount(), 0);
	}


	void DxRenderContext::setProjection(const Mat4x4& proj) {
		D3D11_MAPPED_SUBRESOURCE mapped = {};
		comCheck(m_d3dContext->Map(m_frameBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		
		*static_cast<Mat4x4*>(mapped.pData) = proj;

		m_d3dContext->Unmap(m_frameBuffer, 0);	
	}


	void DxRenderContext::clear(Color col) {
		const FLOAT rgba[] = { col.r, col.g, col.b, col.a };

		ComPtr<ID3D11RenderTargetView> rtv;
		ComPtr<ID3D11DepthStencilView> dsv;

		m_d3dContext->OMGetRenderTargets(1, &rtv, &dsv);
		if(rtv) {
			m_d3dContext->ClearRenderTargetView(rtv, rgba);
		}

		if(dsv) {
			m_d3dContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
		}
	}
}
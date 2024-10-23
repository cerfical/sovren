#include "rhi/dx/DxRenderContext.hpp"

#include "rhi/dx/DxRenderTarget.hpp"
#include "rhi/dx/DxVertexBuffer.hpp"

#include "rhi/dx/ShaderCode.hpp"

namespace sovren::rhi::dx {
    DxRenderContext::DxRenderContext(ID2D1DeviceContext* d2dContext, ID3D11DeviceContext* d3dContext)
        : m_d2dContext(d2dContext), m_d3dContext(d3dContext) {

        comCheck(d2dContext->CreateSolidColorBrush(D2D1::ColorF(DefaultDrawColor), &m_drawBrush));


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
        comCheck(d3dDevice->CreateBuffer(&cbDesc, nullptr, &m_objectBuffer));
    }


    void DxRenderContext::startRender(RenderTarget& rt) {
        auto& dxrt = dynamic_cast<DxRenderTarget&>(rt);

        m_d2dContext->SetTarget(dxrt.asImage());
        m_d2dContext->BeginDraw();

        m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_d3dContext->VSSetShader(m_vertexShader, nullptr, 0);

        m_d3dContext->VSSetConstantBuffers(0, 1, &m_objectBuffer.p);
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

        // by default, perform no transformations
        m_transform3d = Mat4x4::identity();
    }


    void DxRenderContext::endRender() {
        comCheck(m_d2dContext->EndDraw());
        m_d2dContext->SetTarget(nullptr);
    }


    void DxRenderContext::drawMesh(const VertexBuffer& vert) {
        // ensure that constant buffers are written once per change
        if(m_transform3d) {
            writeCb(m_objectBuffer, *m_transform3d);
            m_transform3d.reset();
        }

        const auto& dxvert = dynamic_cast<const DxVertexBuffer&>(vert);

        const auto vb = dxvert.vertexData();
        const UINT stride = dxvert.vertexStride(), offset = 0;

        m_d3dContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
        m_d3dContext->IASetInputLayout(dxvert.inputLayout());

        m_d3dContext->Draw(dxvert.vertexCount(), 0);
    }


    void DxRenderContext::writeCb(ID3D11Buffer* cb, const Mat4x4& mat) {
        D3D11_MAPPED_SUBRESOURCE mapped = {};
        comCheck(m_d3dContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
        *static_cast<Mat4x4*>(mapped.pData) = mat;
        m_d3dContext->Unmap(cb, 0);
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


    void DxRenderContext::setTransformMatrix(const Mat3x3& mat) {
        // Direct2D only supports affine transformations, so ignore the last column values
        m_d2dContext->SetTransform({ mat[0][0], mat[0][1], mat[1][0], mat[1][1], mat[2][0], mat[2][1] });
    }
}

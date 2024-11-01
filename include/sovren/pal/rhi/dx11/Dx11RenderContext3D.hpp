#pragma once

#include "../RenderContext3D.hpp"

#include "Dx11RenderTarget.hpp"
#include "Dx11VertexBuffer.hpp"
#include "ShaderCode.hpp"
#include "com_util.hpp"

#include <d2d1_1.h>
#include <d3d11.h>

#include <array>
#include <optional>

namespace sovren::rhi::dx11 {

    class Dx11RenderContext : public RenderContext3D {
    public:

        explicit Dx11RenderContext(ID3D11DeviceContext* d3dContext)
            : d3dContext_(d3dContext) {

            ComPtr<ID3D11Device> d3dDevice;
            d3dContext->GetDevice(&d3dDevice);

            const auto vs = ShaderCode::vertexShader();
            comCheck(d3dDevice->CreateVertexShader(vs.data(), vs.size(), nullptr, &vertexShader_));

            const auto ps = ShaderCode::pixelShader();
            comCheck(d3dDevice->CreatePixelShader(ps.data(), ps.size(), nullptr, &pixelShader_));


            const D3D11_BUFFER_DESC cbDesc = {
                .ByteWidth = sizeof(Mat4x4),
                .Usage = D3D11_USAGE_DYNAMIC,
                .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
                .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
                .MiscFlags = 0,
                .StructureByteStride = 0
            };
            comCheck(d3dDevice->CreateBuffer(&cbDesc, nullptr, &objectBuffer_));
        }


        void setTarget(RenderTarget& rt) override {
            auto& dxrt = dynamic_cast<Dx11RenderTarget&>(rt);
            auto* const rtv = dxrt.asRenderView();
            d3dContext_->OMSetRenderTargets(1, &rtv, dxrt.asDepthView());

            const D3D11_VIEWPORT viewport = {
                .TopLeftX = 0,
                .TopLeftY = 0,
                .Width = static_cast<FLOAT>(dxrt.width()),
                .Height = static_cast<FLOAT>(dxrt.height()),
                .MinDepth = 0,
                .MaxDepth = 1
            };
            d3dContext_->RSSetViewports(1, &viewport);
        }


        void startDraw() override {
            d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            d3dContext_->VSSetShader(vertexShader_, nullptr, 0);

            d3dContext_->VSSetConstantBuffers(0, 1, &objectBuffer_.p);
            d3dContext_->PSSetShader(pixelShader_, nullptr, 0);

            // by default, perform no transformations
            transform3d_ = Mat4x4::identity();
        }


        void endDraw() override {}


        void setTransform(const Mat4x4& mat) override {
            transform3d_ = mat;
        }


        void drawMesh(const VertexBuffer& vert) override {
            // ensure that constant buffers are written once per change
            if(transform3d_) {
                writeCb(objectBuffer_, *transform3d_);
                transform3d_.reset();
            }

            const auto& dxvert = dynamic_cast<const Dx11VertexBuffer&>(vert);
            auto* const vb = dxvert.vertexData();

            const auto stride = dxvert.vertexStride();
            const UINT offset = 0;

            d3dContext_->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
            d3dContext_->IASetInputLayout(dxvert.inputLayout());

            d3dContext_->Draw(dxvert.vertexCount(), 0);
        }


        void clear(Color col) override {
            ComPtr<ID3D11RenderTargetView> rtv;
            ComPtr<ID3D11DepthStencilView> dsv;

            d3dContext_->OMGetRenderTargets(1, &rtv, &dsv);
            if(rtv) {
                const auto colVec = col.toVec();
                const std::array<FLOAT, 4> rgba = { colVec.x, colVec.y, colVec.z, colVec.w };
                d3dContext_->ClearRenderTargetView(rtv, rgba.data());
            }

            if(dsv) {
                d3dContext_->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1, 0);
            }
        }


    private:
        void writeCb(ID3D11Buffer* cb, const Mat4x4& mat) {
            D3D11_MAPPED_SUBRESOURCE mapped = {};
            comCheck(d3dContext_->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
            *static_cast<Mat4x4*>(mapped.pData) = mat;
            d3dContext_->Unmap(cb, 0);
        }


        ComPtr<ID3D11Buffer> objectBuffer_;

        ComPtr<ID3D11DeviceContext> d3dContext_;
        ComPtr<ID3D11VertexShader> vertexShader_;
        ComPtr<ID3D11PixelShader> pixelShader_;

        std::optional<Mat4x4> transform3d_;
    };

}

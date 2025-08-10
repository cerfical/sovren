#pragma once

#include <sovren/rhi/Context3D.hpp>

#include <sovren/rhi/dx11/Dx11Buffer.hpp>
#include <sovren/rhi/dx11/Dx11Pipeline.hpp>
#include <sovren/rhi/dx11/Dx11Texture.hpp>
#include <sovren/rhi/dx11/com_util.hpp>

#include <d3d11.h>

#include <array>

namespace sovren::rhi {

    class Dx11Context3D : public Context3D {
    public:
        explicit Dx11Context3D(ComPtr<ID3D11DeviceContext> d3dContext)
            : d3dContext_(std::move(d3dContext)) {
        }

        void setPipeline(Pipeline* p) override {
            ID3D11InputLayout* inputLayout = nullptr;
            ID3D11VertexShader* vertexShader = nullptr;
            ID3D11PixelShader* pixelShader = nullptr;

            if(p) {
                const auto& dxp = dynamic_cast<Dx11Pipeline&>(*p);
                inputLayout = dxp.inputLayout;
                vertexShader = dxp.vertexShader;
                pixelShader = dxp.pixelShader;
            }

            d3dContext_->IASetInputLayout(inputLayout);
            d3dContext_->VSSetShader(vertexShader, nullptr, 0);
            d3dContext_->PSSetShader(pixelShader, nullptr, 0);
        }

        void setRenderTarget(Texture* rt) override {
            ID3D11RenderTargetView* rtv = nullptr;
            if(rt) {
                auto& dxt = dynamic_cast<Dx11Texture&>(*rt);
                if(!dxt.rtv) {
                    ComPtr<ID3D11Device> d3dDevice;
                    d3dContext_->GetDevice(&d3dDevice);
                    comCheck(d3dDevice->CreateRenderTargetView(dxt.texture, nullptr, &dxt.rtv));
                }
                rtv = dxt.rtv;
            }
            d3dContext_->OMSetRenderTargets(1, &rtv, nullptr);
        }

        void setVertexBuffer(Buffer* vb, std::size_t stride) override {
            ID3D11Buffer* vertexBuffer = nullptr;
            if(vb) {
                const auto& dxBuf = dynamic_cast<Dx11Buffer&>(*vb);
                vertexBuffer = dxBuf.buffer;
            }

            const UINT strides = static_cast<UINT>(stride);
            const UINT offsets = 0;

            d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
            d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        }

        void setConstantBuffer(Buffer* cb, ShaderStages stages) override {
            ID3D11Buffer* constantBuffer = nullptr;
            if(cb) {
                const auto& dxBuf = dynamic_cast<Dx11Buffer&>(*cb);
                constantBuffer = dxBuf.buffer;
            }

            if((stages & ShaderStages::Vertex) != ShaderStages()) {
                d3dContext_->VSSetConstantBuffers(0, 1, &constantBuffer);
            }
            if((stages & ShaderStages::Pixel) != ShaderStages()) {
                d3dContext_->PSSetConstantBuffers(0, 1, &constantBuffer);
            }
        }

        void setViewport(const Viewport& v) override {
            const D3D11_VIEWPORT d3dViewport = {
                .TopLeftX = v.topLeft.x,
                .TopLeftY = v.topLeft.y,
                .Width = v.size.width,
                .Height = v.size.height,
                .MinDepth = 0,
                .MaxDepth = 1
            };
            d3dContext_->RSSetViewports(1, &d3dViewport);
        }

        void clear(Color c) override {
            ComPtr<ID3D11RenderTargetView> rtv;
            d3dContext_->OMGetRenderTargets(1, &rtv, nullptr);

            if(rtv) {
                const auto rgba = static_cast<std::array<FLOAT, 4>>(c);
                d3dContext_->ClearRenderTargetView(rtv, rgba.data());
            }
        }

        void draw(std::size_t vertexCount) override {
            d3dContext_->Draw(static_cast<UINT>(vertexCount), 0);
        }

        auto map(Buffer* buf, AccessModes access) -> std::span<std::byte> override {
            const auto& dxBuf = dynamic_cast<Dx11Buffer&>(*buf);

            D3D11_MAPPED_SUBRESOURCE mapped = {};
            comCheck(d3dContext_->Map(dxBuf.buffer, 0, toMapFlag(access), 0, &mapped));
            return { static_cast<std::byte*>(mapped.pData), dxBuf.size };
        }

        void unmap(Buffer* buf) override {
            const auto& dxBuf = dynamic_cast<Dx11Buffer&>(*buf);
            d3dContext_->Unmap(dxBuf.buffer, 0);
        }

    private:
        static auto toMapFlag(AccessModes access) noexcept -> D3D11_MAP {
            const auto isRead = (access & AccessModes::Read) != AccessModes();
            const auto isWrite = (access & AccessModes::Write) != AccessModes();

            if(isRead && isWrite) {
                return D3D11_MAP_READ_WRITE;
            }
            if(isRead) {
                return D3D11_MAP_READ;
            }
            if(isWrite) {
                return D3D11_MAP_WRITE_DISCARD;
            }
            return static_cast<D3D11_MAP>(0);
        }

        ComPtr<ID3D11DeviceContext> d3dContext_;
    };

}

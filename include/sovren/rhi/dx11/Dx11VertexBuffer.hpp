#pragma once

#include "../VertexBuffer.hpp"

#include "ShaderCode.hpp"
#include "com_util.hpp"

#include <d3d11.h>
#include <span>

namespace sovren::dx11 {

    class Dx11VertexBuffer : public VertexBuffer {
    public:

        Dx11VertexBuffer(std::span<const std::byte> data, ID3D11Device* device)
            : vertexStride_(3 * 4), vertexCount_(static_cast<UINT>(data.size() / vertexStride_)) {

            const D3D11_INPUT_ELEMENT_DESC elems = {
                .SemanticName = "position",
                .SemanticIndex = 0,
                .Format = DXGI_FORMAT_R32G32B32_FLOAT,
                .InputSlot = 0,
                .AlignedByteOffset = 0,
                .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
                .InstanceDataStepRate = 0
            };

            const auto vs = ShaderCode::vertexShader();
            comCheck(device->CreateInputLayout(
                &elems,
                1,
                vs.data(),
                vs.size(),
                &inputLayout_
            ));


            const D3D11_BUFFER_DESC bufDesc = {
                .ByteWidth = vertexCount_ * vertexStride_,
                .Usage = D3D11_USAGE_IMMUTABLE,
                .BindFlags = D3D11_BIND_VERTEX_BUFFER,
                .CPUAccessFlags = 0,
                .MiscFlags = 0,
                .StructureByteStride = 0
            };

            const D3D11_SUBRESOURCE_DATA bufData = {
                .pSysMem = data.data(),
                .SysMemPitch = 0,
                .SysMemSlicePitch = 0
            };

            comCheck(device->CreateBuffer(&bufDesc, &bufData, &vertexData_));
        }


        [[nodiscard]]
        auto vertexData() const noexcept -> ID3D11Buffer* {
            return vertexData_;
        }


        [[nodiscard]]
        auto inputLayout() const noexcept -> ID3D11InputLayout* {
            return inputLayout_;
        }


        [[nodiscard]]
        auto vertexStride() const noexcept -> UINT {
            return vertexStride_;
        }


        [[nodiscard]]
        auto vertexCount() const noexcept -> UINT {
            return vertexCount_;
        }


    private:
        ComPtr<ID3D11InputLayout> inputLayout_;
        ComPtr<ID3D11Buffer> vertexData_;

        UINT vertexStride_ = {};
        UINT vertexCount_ = {};
    };

}

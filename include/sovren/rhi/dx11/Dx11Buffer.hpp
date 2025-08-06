#pragma once

#include "../Buffer.hpp"
#include "../BufferDesc.hpp"

#include "com_util.hpp"

#include <d3d11.h>

namespace sovren::rhi {

    class Dx11Buffer : public Buffer {
    public:
        explicit Dx11Buffer(const BufferDesc& desc, ID3D11Device* device) {
            D3D11_BUFFER_DESC dxDesc = {};
            if(desc.cpuAccess != AccessModes()) {
                if((desc.cpuAccess & AccessModes::Write) != AccessModes()) {
                    dxDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
                    dxDesc.Usage = D3D11_USAGE_DYNAMIC;
                }
                if((desc.cpuAccess & AccessModes::Read) != AccessModes()) {
                    dxDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
                    dxDesc.Usage = D3D11_USAGE_STAGING;
                }
            } else {
                if((desc.gpuAccess & AccessModes::Read) != AccessModes()) {
                    dxDesc.Usage = D3D11_USAGE_IMMUTABLE;
                }
                if((desc.gpuAccess & AccessModes::Write) != AccessModes()) {
                    dxDesc.Usage = D3D11_USAGE_DEFAULT;
                }
            }

            // if the size of initial data is not 0, use it as the size of the buffer;
            // otherwise, use the specified buffer size
            size = desc.data.size() > 0 ? desc.data.size() : desc.size;
            dxDesc.ByteWidth = static_cast<UINT>(size);
            dxDesc.BindFlags = toBindFlags(desc.bind);

            if(desc.data.size() > 0) {
                // fill the buffer with the initial data, if there is any
                const D3D11_SUBRESOURCE_DATA bufData = {
                    .pSysMem = desc.data.data()
                };
                comCheck(device->CreateBuffer(&dxDesc, &bufData, &buffer));
            } else {
                comCheck(device->CreateBuffer(&dxDesc, nullptr, &buffer));
            }
        }

        ComPtr<ID3D11Buffer> buffer;
        std::size_t size = {};

    private:
        static auto toBindFlags(BindModes bind) noexcept -> D3D11_BIND_FLAG {
            int d3dBind = {};
            if((bind & BindModes::VertexBuffer) != BindModes()) {
                d3dBind |= D3D11_BIND_VERTEX_BUFFER;
            }
            if((bind & BindModes::IndexBuffer) != BindModes()) {
                d3dBind |= D3D11_BIND_INDEX_BUFFER;
            }
            if((bind & BindModes::ConstantBuffer) != BindModes()) {
                d3dBind |= D3D11_BIND_CONSTANT_BUFFER;
            }
            return static_cast<D3D11_BIND_FLAG>(d3dBind);
        }
    };

}

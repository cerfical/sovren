#pragma once

#include "../Pipeline.hpp"
#include "../PipelineDesc.hpp"

#include "com_util.hpp"

#include <d3d11.h>
#include <span>
#include <stdexcept>
#include <vector>

namespace sovren::rhi {

    class Dx11Pipeline : public Pipeline {
    public:
        explicit Dx11Pipeline(const PipelineDesc& desc, ID3D11Device* device)
            : vertexShader(createVertexShader(desc.vertexShader, device))
            , pixelShader(createPixelShader(desc.pixelShader, device))
            , inputLayout(createInputLayout(desc.vertexLayout, desc.vertexShader.data, device)) {}

        ComPtr<ID3D11VertexShader> vertexShader;
        ComPtr<ID3D11PixelShader> pixelShader;
        ComPtr<ID3D11InputLayout> inputLayout;

    private:
        static auto createPixelShader(
            const ShaderCode& code, ID3D11Device* device
        ) -> ComPtr<ID3D11PixelShader> {
            if(code.lang != ShaderLang::CompiledHLSL) {
                throw std::invalid_argument(std::format("unsupported shader language: {}", code.lang));
            }

            ComPtr<ID3D11PixelShader> shader;
            comCheck(device->CreatePixelShader(code.data.data(), code.data.size(), nullptr, &shader));
            return shader;
        }

        static auto createVertexShader(
            const ShaderCode& code, ID3D11Device* device
        ) -> ComPtr<ID3D11VertexShader> {
            if(code.lang != ShaderLang::CompiledHLSL) {
                throw std::invalid_argument(std::format("unsupported shader language: {}", code.lang));
            }

            ComPtr<ID3D11VertexShader> shader;
            comCheck(device->CreateVertexShader(code.data.data(), code.data.size(), nullptr, &shader));
            return shader;
        }

        static auto createInputLayout(
            std::span<const VertexAttr> attrs,
            std::span<const std::byte> vertexShader,
            ID3D11Device* device
        ) -> ComPtr<ID3D11InputLayout> {
            std::vector<D3D11_INPUT_ELEMENT_DESC> elems;
            elems.reserve(attrs.size());

            std::vector<std::string> names;
            names.reserve(attrs.size());

            for(const auto& attr : attrs) {
                const auto& name = names.emplace_back(attr.name);
                elems.push_back({
                    .SemanticName = name.c_str(),
                    .SemanticIndex = 0,
                    .Format = toDxgiFormat(attr.format),
                    .InputSlot = 0,
                    .AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT,
                    .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
                    .InstanceDataStepRate = 0,
                });
            }

            ComPtr<ID3D11InputLayout> layout;
            comCheck(device->CreateInputLayout(
                elems.data(),
                static_cast<UINT>(elems.size()),
                vertexShader.data(),
                vertexShader.size(),
                &layout
            ));
            return layout;
        }

        static auto toDxgiFormat(Format fmt) -> DXGI_FORMAT {
            switch(fmt) {
                case Format::Float3: {
                    return DXGI_FORMAT_R32G32B32_FLOAT;
                }
                default: {
                    throw std::invalid_argument(std::format("unsupported format: {}", fmt));
                }
            }
        }
    };

}

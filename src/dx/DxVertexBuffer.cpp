#include "rhi/dx/DxVertexBuffer.hpp"
#include "rhi/dx/ShaderCode.hpp"

namespace reni::rhi::dx {
	DxVertexBuffer::DxVertexBuffer(std::span<const std::byte> data, ID3D11Device* device) {
		const D3D11_INPUT_ELEMENT_DESC elems[] = { {
			.SemanticName = "position",
			.SemanticIndex = 0,
			.Format = DXGI_FORMAT_R32G32B32_FLOAT,
			.InputSlot = 0,
			.AlignedByteOffset = 0,
			.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
			.InstanceDataStepRate = 0
		}};
		
		m_vertexStride = 4 + 4 + 4;
		m_vertexCount = static_cast<UINT>(data.size() / m_vertexStride);

		const auto vs = ShaderCode::vertexShader();
		comCheck(device->CreateInputLayout(
			elems, sizeof(elems) / sizeof(elems[0]),
			vs.data(), vs.size(),
			&m_inputLayout
		));


		const D3D11_BUFFER_DESC bufDesc = {
			.ByteWidth = m_vertexCount * m_vertexStride,
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

		comCheck(device->CreateBuffer(&bufDesc, &bufData, &m_vertexData));
	}
}
#pragma once

#include "../VertexBuffer.hpp"
#include "util.hpp"

#include <d3d11.h>
#include <span>

namespace reni::rhi::dx {

	class DxVertexBuffer : public VertexBuffer {
	public:

		DxVertexBuffer(std::span<const std::byte> data, ID3D11Device* device);


		ID3D11Buffer* vertexData() const noexcept {
			return m_vertexData;
		}


		ID3D11InputLayout* inputLayout() const noexcept {
			return m_inputLayout;
		}


		UINT vertexStride() const noexcept {
			return m_vertexStride;
		}


		UINT vertexCount() const noexcept {
			return m_vertexCount;
		}


	private:
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_vertexData;

		UINT m_vertexCount = {};
		UINT m_vertexStride = {};
	};

}
#pragma once

#include "../RenderTarget.hpp"
#include "util.hpp"

#include <d2d1_1.h>

namespace reni::rhi::dx {

	class DxRenderTarget : public RenderTarget {
	public:

		DxRenderTarget(IDXGISurface* renderSurface, ID2D1DeviceContext* d2dContext);

		ID2D1Image* asImage() const noexcept {
			return m_d2dRenderImage;
		}

	private:
		ComPtr<ID2D1Image> m_d2dRenderImage;
	};

}
#ifndef RENI_RHI_DX_UTIL_HEADER
#define RENI_RHI_DX_UTIL_HEADER

#include "../../util/types.hpp"

#include <system_error>

#include <atlbase.h>
#include <d2d1.h>

namespace reni::rhi::dx {

	template <typename I>
		requires std::derived_from<I, IUnknown>
	using ComPtr = ATL::CComPtr<I>;


	inline HRESULT safeApiCall(HRESULT hr) {
		if(FAILED(hr)) {
			throw std::system_error(hr, std::system_category());
		}
		return hr;
	}


	inline D2D1_POINT_2F makePoint(Point2 p) {
		return { static_cast<FLOAT>(p.x), static_cast<FLOAT>(p.y) };
	}


	inline D2D1_COLOR_F makeColor(Color c) {
		return { c.red / 255.0f, c.green / 255.0f, c.blue / 255.0f, c.alpha / 255.0f };
	}

}

#endif
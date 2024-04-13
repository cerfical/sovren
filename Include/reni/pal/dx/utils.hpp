#ifndef RENI_PAL_DX_UTILS_HEADER
#define RENI_PAL_DX_UTILS_HEADER

#include "../../Point2D.hpp"
#include "../../Color.hpp"

#include "../win32/utils.hpp"

#include <atlbase.h>
#include <d2d1.h>

namespace reni::pal::dx {

	template <typename I>
		requires std::derived_from<I, IUnknown>
	using ComPtr = ATL::CComPtr<I>;


	inline HRESULT safeApiCall(HRESULT hr) {
		if(FAILED(hr)) {
			win32::raiseError(hr);
		}
		return hr;
	}


	inline D2D1_POINT_2F makePoint(Point2D p) {
		return { static_cast<FLOAT>(p.x), static_cast<FLOAT>(p.y) };
	}


	inline D2D1_COLOR_F makeColor(Color c) {
		return { c.red / 255.0f, c.green / 255.0f, c.blue / 255.0f, c.alpha / 255.0f };
	}

}

#endif
#ifndef RENI_DX_UTILS_HEADER
#define RENI_DX_UTILS_HEADER

#include "../Win32/utils.hpp"
#include "Point2D.hpp"

#include <atlbase.h>
#include <d3d11.h>
#include <d2d1.h>

#include <gsl/narrow>

namespace RENI {

	template <typename T> using ComPtr = ATL::CComPtr<T>;


	inline HRESULT comCheck(HRESULT hr) {
		if(FAILED(hr)) {
			win32Error(hr);
		}
		return hr;
	}


	inline auto toPoint2F(Point2D p) noexcept {
		return D2D1::Point2F(
			gsl::narrow_cast<FLOAT>(p.x),
			gsl::narrow_cast<FLOAT>(p.y)
		);
	}

	inline auto toColorF(Color c) noexcept {
		return D2D1::ColorF(c.red / 255.0f, c.green / 255.0f, c.blue / 255.0f, c.alpha / 255.0f);
	}

}

#endif
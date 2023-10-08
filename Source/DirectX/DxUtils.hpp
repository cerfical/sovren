#ifndef RENI_DX_UTILS_HEADER
#define RENI_DX_UTILS_HEADER

#include "WinUtils.hpp"
#include "Utils.hpp"

#include <atlbase.h>
#include <d3d11.h>
#include <d2d1.h>

#include <gsl/util>

namespace RENI {
	template <typename T> using ComPtr = ATL::CComPtr<T>;


	inline HRESULT comCheck(HRESULT hr) {
		if(FAILED(hr)) {
			raiseWin32Error(hr);
		}
		return hr;
	}


	inline auto toRectF(const Rect2D& rect) noexcept {
		return D2D1::RectF(
			gsl::narrow_cast<FLOAT>(rect.left()),
			gsl::narrow_cast<FLOAT>(rect.top()),
			gsl::narrow_cast<FLOAT>(rect.right()),
			gsl::narrow_cast<FLOAT>(rect.bottom())
		);
	}

	inline auto toSizeU(const Size2D& size) noexcept {
		return D2D1::SizeU(
			gsl::narrow_cast<UINT32>(size.width()),
			gsl::narrow_cast<UINT32>(size.height())
		);
	}

	inline auto toPoint2F(const Point2D& point) noexcept {
		return D2D1::Point2F(
			gsl::narrow_cast<FLOAT>(point.x()),
			gsl::narrow_cast<FLOAT>(point.y())
		);
	}


	inline auto makeColorF(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha) noexcept {
		return D2D1::ColorF(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f);
	}

	inline auto toColorF(Color color) noexcept {
		return makeColorF(color.red(), color.green(), color.blue(), color.alpha());
	}
}

#endif
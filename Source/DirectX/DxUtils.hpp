#ifndef RENI_DX_UTILS_HEADER
#define RENI_DX_UTILS_HEADER

#include "WinUtils.hpp"
#include "Utils.hpp"

#include <atlbase.h>
#include <d3d11.h>
#include <d2d1.h>

#include <gsl/util>

namespace RENI {
	template <typename T>
	using ComPtr = ATL::CComPtr<T>;



	template <typename C, typename... Args>
		requires std::same_as<std::invoke_result_t<C, Args...>, HRESULT>
			&& std::invocable<C, Args...>
	void safeComApiCall(C&& func, Args&&... args) {
		const auto result = std::invoke(std::forward<C>(func), std::forward<Args>(args)...);
		if(FAILED(result)) {
			raiseWin32Error(result);
		}
	}



	inline auto makeRectF(const Rect2D& rect) noexcept {
		return D2D1::RectF(
			gsl::narrow_cast<FLOAT>(rect.left()),
			gsl::narrow_cast<FLOAT>(rect.top()),
			gsl::narrow_cast<FLOAT>(rect.right()),
			gsl::narrow_cast<FLOAT>(rect.bottom())
		);
	}

	inline auto makeSizeU(const Size2D& size) noexcept {
		return D2D1::SizeU(
			gsl::narrow_cast<UINT32>(size.width()),
			gsl::narrow_cast<UINT32>(size.height())
		);
	}

	inline auto makePoint2F(const Point2D& point) noexcept {
		return D2D1::Point2F(
			gsl::narrow_cast<FLOAT>(point.x()),
			gsl::narrow_cast<FLOAT>(point.y())
		);
	}

	inline auto makeColorF(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept {
		return D2D1::ColorF(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	inline auto makeColorF(Color color) noexcept {
		return makeColorF(color.red(), color.green(), color.blue(), color.alpha());
	}
}

#endif
#ifndef RENI_D3D_UTILS_HEADER
#define RENI_D3D_UTILS_HEADER

#include "../Win32/WinUtils.hpp"
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
	void SafeComApiCall(C&& func, Args&&... args) {
		const auto result = std::invoke(std::forward<C>(func), std::forward<Args>(args)...);
		if(FAILED(result)) {
			RaiseWin32Error(result);
		}
	}



	inline auto MakeRectF(const Rect2D& rect) noexcept {
		return D2D1::RectF(
			gsl::narrow_cast<FLOAT>(rect.GetLeft()),
			gsl::narrow_cast<FLOAT>(rect.GetTop()),
			gsl::narrow_cast<FLOAT>(rect.GetRight()),
			gsl::narrow_cast<FLOAT>(rect.GetBottom())
		);
	}

	inline auto MakeSizeU(const Size2D& size) noexcept {
		return D2D1::SizeU(
			gsl::narrow_cast<UINT32>(size.GetWidth()),
			gsl::narrow_cast<UINT32>(size.GetHeight())
		);
	}

	inline auto MakePoint2F(const Point2D& point) noexcept {
		return D2D1::Point2F(
			gsl::narrow_cast<FLOAT>(point.GetX()),
			gsl::narrow_cast<FLOAT>(point.GetY())
		);
	}

	inline auto MakeColorF(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept {
		return D2D1::ColorF(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}

	inline auto MakeColorF(Color color) noexcept {
		return MakeColorF(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
	}
}

#endif
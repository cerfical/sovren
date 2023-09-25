#ifndef RENI_D2D_UTILS_HEADER
#define RENI_D2D_UTILS_HEADER

#include "WinUtils.hpp"
#include <gsl/util>

#include <atlbase.h>
#include <d2d1.h>

namespace RENI {
	/** @brief Shared smart pointer for COM interfaces. */
	template <typename T>
	using ComPtr = ATL::CComPtr<T>;



	/** @brief Make a D2D1_COLOR_F from a Color. */
	inline auto MakeColorF(Color color) noexcept {
		return D2D1::ColorF(
			color.r / 255.0f,
			color.g / 255.0f,
			color.b / 255.0f,
			color.a / 255.0f
		);
	}

	/** @brief Make a D2D1_RECT_F from a Rect2D. */
	inline auto MakeRectF(const Rect2D& rect) noexcept {
		return D2D1::RectF(
			gsl::narrow_cast<FLOAT>(rect.topLeft.x),
			gsl::narrow_cast<FLOAT>(rect.topLeft.y),
			gsl::narrow_cast<FLOAT>(rect.topLeft.x + rect.extent.width),
			gsl::narrow_cast<FLOAT>(rect.topLeft.y + rect.extent.height)
		);
	}

	/** @brief Make a D2D1_SIZE_U from an Extent2D. */
	inline auto MakeSizeU(Extent2D size) noexcept {
		return D2D1::SizeU(
			gsl::narrow_cast<UINT32>(size.width),
			gsl::narrow_cast<UINT32>(size.height)
		);
	}
	
	/** @brief Make a D2D1_POINT_2F from a Point2D. */
	inline auto MakePoint2F(Point2D point) noexcept {
		return D2D1::Point2F(
			gsl::narrow_cast<FLOAT>(point.x),
			gsl::narrow_cast<FLOAT>(point.y)
		);
	}
}

#endif
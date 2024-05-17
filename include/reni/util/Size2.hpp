#pragma once

#include "../math/Vec2.hpp"
#include <compare>

namespace reni {

	/**
	 * @brief Dimensions of a rectangular area in 2D space.
	*/
	struct Size2 {

		friend auto operator<=>(Size2 lhs, Size2 rhs) noexcept = default;


		Size2() noexcept = default;

		Size2(Vec2 v) noexcept
			: Size2(static_cast<int>(v.x), static_cast<int>(v.y)) {}

		Size2(int width, int height) noexcept
			: width(width), height(height) {}


		operator Vec2() const noexcept {
			return Vec2(static_cast<float>(width), static_cast<float>(height));
		}


		int width = {};
		int height = {};
	};

}
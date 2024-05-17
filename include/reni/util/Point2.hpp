#pragma once

#include "../math/Vec2.hpp"
#include <compare>

namespace reni {

	/**
	 * @brief Coordinates of a point in 2D space as a pair of two real numbers.
	*/
	struct Point2 {

		friend auto operator<=>(Point2 lhs, Point2 rhs) noexcept = default;


		Point2() noexcept = default;

		Point2(Vec2 v) noexcept
			: Point2(static_cast<int>(v.x), static_cast<int>(v.y)) {}

		Point2(int x, int y) noexcept
			: x(x), y(y) {}


		operator Vec2() const noexcept {
			return Vec2(static_cast<float>(x), static_cast<float>(y));
		}


		int x = {};
		int y = {};
	};

}
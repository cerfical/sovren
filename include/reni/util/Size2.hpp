#pragma once

#include <compare>

namespace reni {

	/**
	 * @brief Dimensions of a rectangular area in 2D space.
	*/
	struct Size2 {

		friend auto operator<=>(Size2 lhs, Size2 rhs) = default;

		int width = {};
		int height = {};

	};

}
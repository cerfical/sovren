#ifndef RENI_SIZE_2_HEADER
#define RENI_SIZE_2_HEADER

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

#endif
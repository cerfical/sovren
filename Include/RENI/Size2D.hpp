#ifndef RENI_SIZE_2D_HEADER
#define RENI_SIZE_2D_HEADER

#include <compare>
#include <ostream>

namespace reni {

	/**
	 * @brief Dimensions of a rectangular area in 2D space.
	*/
	struct Size2D {

		friend constexpr auto operator<=>(Size2D lhs, Size2D rhs) = default;

		int width = {};
		int height = {};

	};

}

#endif
#ifndef RENI_POINT_2_HEADER
#define RENI_POINT_2_HEADER

#include <compare>

namespace reni {

	/**
	 * @brief Point in 2D space as a pair of two integer coordinates.
	*/
	struct Point2 {

		friend constexpr auto operator<=>(Point2 lhs, Point2 rhs) = default;

		int x = {};
		int y = {};

	};

}

#endif
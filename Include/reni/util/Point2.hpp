#ifndef RENI_POINT_2_HEADER
#define RENI_POINT_2_HEADER

#include <compare>

namespace reni {

	/**
	 * @brief Coordinates of a point in 2D space as a pair of two real numbers.
	*/
	struct Point2 {

		friend auto operator<=>(Point2 lhs, Point2 rhs) = default;

		float x = {};
		float y = {};

	};

}

#endif
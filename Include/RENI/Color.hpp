#ifndef RENI_COLOR_HEADER
#define RENI_COLOR_HEADER

#include <compare>
#include <ostream>

namespace reni {

	/**
	 * @brief RGBA color as four 8-bit values.
	*/
	struct Color {
		friend constexpr auto operator<=>(Color lhs, Color rhs) = default;

		friend std::ostream& operator<<(std::ostream& out, Color c);


		std::uint8_t red = {};
		std::uint8_t green = {};
		std::uint8_t blue = {};
		std::uint8_t alpha = 255;
	};

}

#endif
#pragma once

#include <compare>

namespace reni {

	/**
	 * @brief RGBA color in the form of four real components.
	*/
	struct Color {
		
		friend auto operator<=>(Color lhs, Color rhs) = default;

		float r = {};
		float g = {};
		float b = {};
		float a = 1.0f;

	};

}
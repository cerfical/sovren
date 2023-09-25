#ifndef RENI_UTILS_HEADER
#define RENI_UTILS_HEADER

#include <compare>
#include <cstdint>

namespace RENI {
	/**
	 * @brief Rectangle dimensions as a pair of two values.
	 */
	struct Extent2D {
		/** @brief Compare two Extent2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Extent2D&, const Extent2D&
		) = default;

		std::uintptr_t width = { };
		std::uintptr_t height = { };
	};

	/**
	 * @brief Point in 2D space as a pair of two values.
	 */
	struct Point2D {
		/** @brief Compare two Point2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Point2D&, const Point2D&
		) = default;

		std::intptr_t x = { };
		std::intptr_t y = { };
	};

	/**
	 * @brief Rectangle in 2D space specified as a top left corner and dimensions of two sides.
	 */
	struct Rect2D {
		/** @brief Compare two Rect2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Rect2D&, const Rect2D&
		) = default;

		Point2D topLeft = { };
		Extent2D extent = { };
	};

	/**
	 * @brief Line in 2D space defined by two endpoints.
	 */
	struct Line2D {
		/** @brief Compare two Line2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Line2D&, const Line2D&
		) = default;

		Point2D start = { };
		Point2D end = { };
	};

	/**
	 * @brief RGBA color as four 8-bit values.
	 */
	struct Color {
		/** @brief Compare two @ref Color "Colors" by their components. */
		friend constexpr auto operator<=>(
			const Color&, const Color&
		) = default;

		std::uint8_t r = { };
		std::uint8_t g = { };
		std::uint8_t b = { };
		std::uint8_t a = 0xff;
	};
}

#endif
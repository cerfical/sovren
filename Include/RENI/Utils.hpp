#ifndef RENI_UTILS_HEADER
#define RENI_UTILS_HEADER

#include <compare>
#include <cstdint>

namespace RENI {
	/**
	 * @brief Represents the dimensions of a rectangle as a pair of two values.
	 */
	struct Extent2D {
		friend constexpr auto operator<=>(
			const Extent2D&, const Extent2D&
		) = default;

		std::size_t width;
		std::size_t height;
	};

	/**
	 * @brief Represents a point in 2D space as a pair of two values.
	 */
	struct Point2D {
		friend constexpr auto operator<=>(
			const Point2D&, const Point2D&
		) = default;

		int x = { };
		int y = { };
	};

	/**
	 * @brief Represents a rectangle in 2D space as the top left point and the dimensions of the two sides.
	 */
	struct Rect {
		friend constexpr auto operator<=>(
			const Rect&, const Rect&
		) = default;

		Point2D topLeft;
		Extent2D extent;
	};

	/**
	 * @brief Represents an RGBA color as four 8-bit values.
	 */
	struct Color {
		/** @brief Make a color from a 32-bit unsigned integer containing the color components in the form RRGGBBAA. */
		constexpr static Color FromInt(std::uint32_t color) noexcept {
			return {
				.r = ((std::uint8_t*)(&color))[3],
				.g = ((std::uint8_t*)(&color))[2],
				.b = ((std::uint8_t*)(&color))[1],
				.a = ((std::uint8_t*)(&color))[0]
			};
		}

		/** @brief Make a color from RGBA components. */
		constexpr static Color FromRgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept {
			return { r, g, b, a };
		}
		/** @brief Make a color from RGB components. */
		constexpr static Color FromRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept {
			return FromRgba(r, g, b, 255);
		}

		friend constexpr auto operator<=>(
			const Color&, const Color&
		) = default;

		std::uint8_t r = { };
		std::uint8_t g = { };
		std::uint8_t b = { };
		std::uint8_t a = { };
	};
}

#endif
#ifndef RENI_UTILS_HEADER
#define RENI_UTILS_HEADER

#include <compare>
#include <cstdint>

#include <ostream>
#include <ranges>
#include <vector>
#include <memory>

namespace RENI {
	/**
	 * @brief Rectangle dimensions as a pair of two integer values.
	 */
	struct Extent2D {
		/** @brief Compare two Extent2D objects by their components. */
		friend constexpr auto operator<=>(const Extent2D&, const Extent2D&) = default;

		/** @brief Print out an Extent2D object to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Extent2D& extent) {
			return out << '(' << extent.width << ", " << extent.height << ')';
		}

		int width = { }; /**< @brief Width of a rectangle. */
		int height = { }; /**< @brief Height of a rectangle. */
	};



	/**
	 * @brief Point in 2D space as a pair of two integer values.
	 */
	struct Point2D {
		/** @brief Compare two points by their coordinates. */
		friend constexpr auto operator<=>(const Point2D&, const Point2D&) = default;

		/** @brief Print out a point to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Point2D& point) {
			return out << '(' << point.x << ", " << point.y << ')';
		}

		int x = { }; /**< @brief x-coordinate of a point. */
		int y = { }; /**< @brief y-coordinate of a point. */
	};



	/**
	 * @brief Displacement between two points as a pair of two integer values.
	 */
	struct Displace2D {
		/** @brief Compare two Displace2D objects by their components. */
		friend constexpr auto operator<=>(const Displace2D&, const Displace2D&) = default;

		/** @brief Print out a Displace2D object to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Displace2D& displace) {
			return out << '(' << displace.dx << ", " << displace.dy << ')';
		}

		int dx = { }; /**< @brief Change in the x coordinate. */
		int dy = { }; /**< @brief Change in the y coordinate. */
	};

	

	/**
	 * @brief Rectangle in 2D space defined with by a top left corner and dimensions of two sides.
	 */
	struct Rect2D {
		/** @brief Compare two rectangles by their components. */
		friend constexpr auto operator<=>(const Rect2D&, const Rect2D&) = default;

		/** @brief Print out a rectangle to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Rect2D& rect) {
			return out << '(' << rect.topLeft << ", " << rect.extent << ')';
		}

		Point2D topLeft = { }; /**< @brief Coordinates of a top left corner of a rectangle. */
		Extent2D extent = { }; /**< @brief Sizes of two sides of a rectangle. */
	};



	/**
	 * @brief Line in 2D space defined by two endpoints.
	 */
	struct Line2D {
		/** @brief Compare two lines by their endpoints. */
		friend constexpr auto operator<=>(const Line2D&, const Line2D&) = default;

		/** @brief Print out a line to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Line2D& line) {
			return out << '(' << line.start << ", " << line.end << ')';
		}

		Point2D start = { }; /**< @brief Coordinates of a start point of a line. */
		Point2D end = { }; /**< @brief Coordinates of an end point of a line. */
	};
	


	/**
	 * @brief RGBA color as four 8-bit values.
	 */
	struct Color {
		/** @brief Compare two colors by their components. */
		friend constexpr auto operator<=>(const Color&, const Color&) = default;

		/** @brief Print out a color to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Color& color);

		std::uint8_t r = { }; /**< @brief Red component of a color. */
		std::uint8_t g = { }; /**< @brief Green component of a color. */
		std::uint8_t b = { }; /**< @brief Blue component of a color. */
		std::uint8_t a = 0xff; /**< @brief Alpha component of a color. */
	};



	/**
	 * @brief Base class for platform specific implementation classes.
	 */
	class ImplBase {
	public:
		/** @{ */
		/** @copybrief */
		ImplBase() = default;

		/** @copybrief */
		virtual ~ImplBase() = default;
		/** @} */
	};

	/**
	 * @brief Holds and manages the lifetime of implementation objects.
	 */
	template <typename Impl>
	class ImplHolder {
	public:
		/** @{ */
		ImplHolder() = default;
		~ImplHolder() = default;
		/** @} */
		
		/** @{ */
		ImplHolder(ImplHolder&&) = default;
		ImplHolder& operator=(ImplHolder&&) = default;
		/** @} */

		/** @{ */
		ImplHolder(const ImplHolder&) = delete;
		ImplHolder& operator=(const ImplHolder&) = delete;
		/** @} */

		/** @{ */
		/** @brief Initialize the pointer. */
		void Init(std::unique_ptr<Impl> ptr) noexcept {
			m_ptr = std::move(ptr);
		}
		/** @} */

		/** @{ */
		/** @brief Read access to the stored implementation object. */
		const Impl* operator->() const noexcept {
			return static_cast<const Impl*>(m_ptr.get());
		}

		/** @brief Write access to the stored implementation object. */
		Impl* operator->() noexcept {
			return static_cast<Impl*>(m_ptr.get());
		}
		/** @} */

	private:
		std::unique_ptr<ImplBase> m_ptr;
	};
}

#endif

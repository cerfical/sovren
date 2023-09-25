#ifndef RENI_UTILS_HEADER
#define RENI_UTILS_HEADER

#include <compare>
#include <cstdint>
#include <cstddef>

#include <algorithm>
#include <ranges>
#include <vector>

#include <ostream>
#include <iomanip>

namespace RENI {
	/**
	 * @brief Rectangle dimensions as a pair of two values.
	 */
	struct Extent2D {
		/** @brief Compare two Extent2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Extent2D&, const Extent2D&
		) = default;

		std::size_t width = { }; /**< @brief Width of a rectangle. */
		std::size_t height = { }; /**< @brief Height of a rectangle. */
	};

	/** @brief Print an Extent2D object to a @c std::ostream. */
	inline std::ostream& operator<<(std::ostream& out, const Extent2D& extent) {
		return out << '(' << extent.width << ", " << extent.height << ')';
	}



	/**
	 * @brief Point in 2D space as a pair of two values.
	 */
	struct Point2D {
		/** @brief Compare two points by their coordinates. */
		friend constexpr auto operator<=>(
			const Point2D&, const Point2D&
		) = default;

		std::ptrdiff_t x = { }; /**< @brief x-coordinate of a point. */
		std::ptrdiff_t y = { }; /**< @brief y-coordinate of a point. */
	};

	/** @brief Print out a point to a @c std::ostream. */
	inline std::ostream& operator<<(std::ostream& out, const Point2D& point) {
		return out << '(' << point.x << ", " << point.y << ')';
	}



	/**
	 * @brief Displacement between two points.
	 */
	struct Displace2D {
		/** @brief Compare two Displace2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Displace2D&, const Displace2D&
		) = default;

		std::ptrdiff_t dx = { }; /**< @brief Change in the x coordinate. */
		std::ptrdiff_t dy = { }; /**< @brief Change in the y coordinate. */
	};

	/** @brief Print a Displace2D object to a @c std::ostream. */
	inline std::ostream& operator<<(std::ostream& out, const Displace2D& displace) {
		return out << '(' << displace.dx << ", " << displace.dy << ')';
	}



	/**
	 * @brief Rectangle in 2D space specified as a top left corner and dimensions of two sides.
	 */
	struct Rect2D {
		/** @brief Compare two rectangles by their constituents. */
		friend constexpr auto operator<=>(
			const Rect2D&, const Rect2D&
		) = default;

		Point2D topLeft = { }; /**< @brief Coordinates of a top left corner of a rectangle. */
		Extent2D extent = { }; /**< @brief Sizes of two sides of a rectangle. */
	};

	/** @brief Print out a rectangle to a @c std::ostream. */
	inline std::ostream& operator<<(std::ostream& out, const Rect2D& rect) {
		return out << '(' << rect.topLeft << ", " << rect.extent << ')';
	}



	/**
	 * @brief Line in 2D space defined by two endpoints.
	 */
	struct Line2D {
		/** @brief Compare two lines by their endpoints. */
		friend constexpr auto operator<=>(
			const Line2D&, const Line2D&
		) = default;

		Point2D start = { }; /**< @brief Coordinates of a start point of a line. */
		Point2D end = { }; /**< @brief Coordinates of an end point of a line. */
	};

	/** @brief Print out a line to a @c std::ostream. */
	inline std::ostream& operator<<(std::ostream& out, const Line2D& line) {
		return out << '(' << line.start << ", " << line.end << ')';
	}



	/**
	 * @brief RGBA color as four 8-bit values.
	 */
	struct Color {
		/** @brief Compare two colors by their components. */
		friend constexpr auto operator<=>(
			const Color&, const Color&
		) = default;

		std::uint8_t r = { }; /**< @brief Red component of a color. */
		std::uint8_t g = { }; /**< @brief Green component of a color. */
		std::uint8_t b = { }; /**< @brief Blue component of a color. */
		std::uint8_t a = 0xff; /**< @brief Alpha component of a color. */
	};

	/** @brief Print out a color to a @c std::ostream. */
	inline std::ostream& operator<<(std::ostream& out, const Color& color) {
		return out << '#' << std::hex << std::setfill('0')
			<< std::setw(2) << static_cast<int>(color.r)
			<< std::setw(2) << static_cast<int>(color.g)
			<< std::setw(2) << static_cast<int>(color.b)
			<< std::setw(2) << static_cast<int>(color.a);
	}



	/**
	 * @brief List of observers waiting for some event to happen.
	 */
	template <typename T>
	class ObserverList {
	public:
		/** @{ */
		/** @brief Construct a new ObserverList. */
		ObserverList() = default;

		/** @brief Destroy the ObserverList. */
		~ObserverList() = default;
		/** @} */


		/** @{ */
		/** @brief Construct a new ObserverList by making a copy of another ObserverList. */
		ObserverList(const ObserverList&) = default;
		
		/** @brief Assign to this ObserverList a copy of another ObserverList. */
		ObserverList& operator=(const ObserverList&) = default;
		/** @} */


		/** @{ */
		/** @brief Construct a new ObserverList by moving from another ObserverList. */
		ObserverList(ObserverList&&) = default;

		/** @brief Move another ObserverList into this ObserverList. */
		ObserverList& operator=(ObserverList&&) = default;
		/** @} */


		/** @{ */
		/** @brief Call the specified event handler on all observers. */
		template <typename... Args, typename... Params>
		void TriggerEvent(void (T::* event)(Params...), Args&&... args) const {
			for(const auto observer : observers) {
				(observer->*event)(std::forward<Args>(args)...);
			}
		}
		/** @} */


		/** @{ */
		/** @brief Add an observer to the list. */
		void Add(T& observer) {
			const auto reversed = std::ranges::reverse_view(observers);
			if(std::ranges::find(reversed, &observer) == reversed.end()) {
				observers.push_back(&observer);
			}
		}

		/** @brief Remove an observer from the list. */
		void Remove(T& observer) noexcept {
			const auto reversed = std::ranges::reverse_view(observers);
			if(const auto it = std::ranges::find(reversed, &observer); it != reversed.end()) {
				observers.erase(std::next(it).base());
			}
		}
		/** @} */

	private:
		std::vector<T*> observers;
	};
}

#endif
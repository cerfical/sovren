#ifndef RENI_UTILS_HEADER
#define RENI_UTILS_HEADER

#include <compare>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <ranges>
#include <vector>

namespace RENI {
	/**
	 * @brief Rectangle dimensions as a pair of two values.
	 */
	struct Extent2D {
		/** @brief Compare two Extent2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Extent2D&, const Extent2D&
		) = default;

		std::size_t width = { };
		std::size_t height = { };
	};

	/**
	 * @brief Point in 2D space as a pair of two values.
	 */
	struct Point2D {
		/** @brief Compare two Point2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Point2D&, const Point2D&
		) = default;

		std::ptrdiff_t x = { };
		std::ptrdiff_t y = { };
	};

	/**
	 * @brief Displacement between two Point2D objects.
	 */
	struct Displace2D {
		/** @brief Compare two Displace2D objects by their constituents. */
		friend constexpr auto operator<=>(
			const Displace2D&, const Displace2D&
		) = default;

		std::ptrdiff_t dx = { };
		std::ptrdiff_t dy = { };
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
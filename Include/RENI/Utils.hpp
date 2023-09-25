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
	class Size2D {
	public:
		/** @brief Compare two sizes by dimensions. */
		friend constexpr auto operator<=>(const Size2D& lhs, const Size2D& rhs) = default;

		/** @brief Print out a size to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Size2D& size);


		/** @{ */
		/** @brief Construct a Size2D from width and height. */
		Size2D(int width = 0, int height = 0) noexcept
			: m_width(width), m_height(height)
		{ }
		/** @} */

		/** @{ */
		Size2D(const Size2D&) = default;
		Size2D& operator=(const Size2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new width for a rectangle. */
		Size2D& width(int w) noexcept {
			m_width = w;
			return *this;
		}

		/** @brief Create a copy of the Size2D object with the specified width. */
		Size2D width(int w) const noexcept {
			return Size2D(*this).width(w);
		}

		/** @brief Width of a rectangle. */
		int width() const noexcept {
			return m_width;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new height for a rectangle. */
		Size2D& height(int h) noexcept {
			m_height = h;
			return *this;
		}

		/** @brief Create a copy of the Size2D object with the specified height. */
		Size2D height(int h) const noexcept {
			return Size2D(*this).height(h);
		}

		/** @brief Height of a rectangle. */
		int height() const noexcept {
			return m_height;
		}
		/** @} */

	private:
		int m_width = { };
		int m_height = { };
	};



	/**
	 * @brief Displacement between two points as a pair of two integer values.
	 */
	class Offset2D {
	public:
		/** @brief Compare two offsets by deltas. */
		friend constexpr auto operator<=>(const Offset2D& lhs, const Offset2D& rhs) = default;

		/** @brief Print out an offset to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Offset2D& offset);


		/** @{ */
		/** @brief Construct an offset from x and y deltas. */
		Offset2D(int dx = 0, int dy = 0) noexcept
			: m_dx(dx), m_dy(dy)
		{ }
		/** @} */

		/** @{ */
		Offset2D(const Offset2D&) = default;
		Offset2D& operator=(const Offset2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new x-delta for the offset. */
		Offset2D& dx(int dx) noexcept {
			m_dx = dx;
			return *this;
		}

		/** @brief Create a copy of the offset with the specified x-delta. */
		Offset2D dx(int dx) const noexcept {
			return Offset2D(*this).dx(dx);
		}

		/** @brief Change in the x coordinate. */
		int dx() const noexcept {
			return m_dx;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new y-delta for the offset. */
		Offset2D& dy(int dy) noexcept {
			m_dy = dy;
			return *this;
		}

		/** @brief Create a copy of the offset with the specified y-delta. */
		Offset2D dy(int dy) const noexcept {
			return Offset2D(*this).dy(dy);
		}

		/** @brief Change in the y coordinate. */
		int dy() const noexcept {
			return m_dy;
		}
		/** @} */

	private:
		int m_dx = { };
		int m_dy = { };
	};



	/**
	 * @brief Point in 2D space as a pair of two integer values.
	 */
	class Point2D {
	public:
		/** @brief Compare two points by coordinates. */
		friend constexpr auto operator<=>(const Point2D& lhs, const Point2D& rhs) = default;

		/** @brief Print out a point to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Point2D& point);

		/** @brief Find the displacement between two points. */
		friend Offset2D operator-(const Point2D& lhs, const Point2D& rhs) noexcept {
			return Offset2D(lhs.x() - rhs.x(), lhs.y() - rhs.y());
		}

		/** @brief Move a point by the given offset. */
		friend Point2D operator+(const Point2D& p, const Offset2D& o) noexcept {
			return Point2D(p.x() + o.dx(), p.y() + o.dy());
		}


		/** @{ */
		/** @brief Construct a point from x and y coordinates. */
		Point2D(int x = 0, int y = 0) noexcept
			: m_x(x), m_y(y)
		{ }
		/** @} */
		
		/** @{ */
		Point2D(const Point2D&) = default;
		Point2D& operator=(const Point2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new x-coordinate for the point. */
		Point2D& x(int x) noexcept {
			m_x = x;
			return *this;
		}

		/** @brief Create a copy of the point with the specified x-coordinate. */
		Point2D x(int x) const noexcept {
			return Point2D(*this).x(x);
		}

		/** @brief x-coordinate of the point. */
		int x() const noexcept {
			return m_x;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new y-coordinate for the point. */
		Point2D& y(int y) noexcept {
			m_y = y;
			return *this;
		}

		/** @brief Create a copy of the point with the specified y-coordinate. */
		Point2D y(int y) const noexcept {
			return Point2D(*this).y(y);
		}

		/** @brief y-coordinate of the point. */
		int y() const noexcept {
			return m_y;
		}
		/** @} */

	private:
		int m_x = { };
		int m_y = { };
	};

	

	/**
	 * @brief Rectangle in 2D space defined by two points.
	 */
	class Rect2D {
	public:
		/** @brief Compare two rectangles by points. */
		friend constexpr auto operator<=>(const Rect2D& lhs, const Rect2D& rhs) = default;

		/** @brief Print out a rectangle to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Rect2D& rect);


		/** @{ */
		Rect2D() = default;

		/** @brief Construct a rectangle from its top left and bottom right points. */
		Rect2D(const Point2D& topLeft, const Point2D& bottomRight) noexcept
			: m_topLeft(topLeft), m_bottomRight(bottomRight)
		{ }

		/** @brief Construct a rectangle from its top left point and sizes of the two sides. */
		Rect2D(const Point2D& topLeft, const Size2D& size) noexcept
			: m_topLeft(topLeft) {
			this->size(size);
		}
		/** @{ */

		/** @{ */
		Rect2D(const Rect2D&) = default;
		Rect2D& operator=(const Rect2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new x-coordinate for the top left corner of the rectangle. */
		Rect2D& left(int l) noexcept {
			topLeft().x(l);
			return *this;
		}

		/** @brief Create a copy of the rectangle with the specified left coordinate. */
		Rect2D left(int l) const noexcept {
			return Rect2D(*this).left(l);
		}

		/** @brief x-coordinate of the top left corner of the rectangle. */
		int left() const noexcept {
			return topLeft().x();
		}
		/** @{ */


		/** @{ */
		/** @brief Set the new y-coordinate for the top left corner of the rectangle. */
		Rect2D& top(int t) noexcept {
			topLeft().y(t);
			return *this;
		}

		/** @brief Create a copy of the rectangle with the specified top coordinate. */
		Rect2D top(int t) const noexcept {
			return Rect2D(*this).top(t);
		}

		/** @brief y-coordinate of the top left corner of the rectangle. */
		int top() const noexcept {
			return topLeft().y();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new x-coordinate for the bottom right corner of the rectangle. */
		Rect2D& right(int r) noexcept {
			bottomRight().x(r);
			return *this;
		}

		/** @brief Create a copy of the rectangle with the specified right coordinate. */
		Rect2D right(int r) const noexcept {
			return Rect2D(*this).right(r);
		}

		/** @brief x-coordinate of the bottom right corner of the rectangle. */
		int right() const noexcept {
			return bottomRight().x();
		}
		/** @{ */


		/** @{ */
		/** @brief Set the new y-coordinate for the bottom right corner of the rectangle. */
		Rect2D& bottom(int b) noexcept {
			bottomRight().y(b);
			return *this;
		}

		/** @brief Create a copy of the rectangle with the specified bottom coordinate. */
		Rect2D bottom(int b) const noexcept {
			return Rect2D(*this).bottom(b);
		}

		/** @brief y-coordinate of the bottom right corner of the rectangle. */
		int bottom() const noexcept {
			return bottomRight().y();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new width for the rectangle. */
		Rect2D& width(int w) noexcept {
			m_bottomRight.x(topLeft().x() + w);
			return *this;
		}

		/** @brief Create a copy of the rectangle with the specified width. */
		Rect2D width(int w) const noexcept {
			return Rect2D(*this).width(w);
		}

		/** @brief Width of the rectangle. */
		int width() const noexcept {
			return size().width();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new height for the rectangle. */
		Rect2D& height(int h) noexcept {
			m_bottomRight.y(topLeft().y() + h);
			return *this;
		}

		/** @brief Create a copy of the rectangle with the specified height. */
		Rect2D height(int h) const noexcept {
			return Rect2D(*this).height(h);
		}

		/** @brief Height of the rectangle. */
		int height() const noexcept {
			return size().height();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new top left point for the rectangle. */
		Rect2D& topLeft(const Point2D& p) noexcept {
			m_topLeft = p;
			return *this;
		}

		/** @brief Coordinates of the top left corner of the rectangle. */
		Point2D& topLeft() noexcept {
			return m_topLeft;
		}


		/** @brief Create a copy of the rectangle with the specified top left point. */
		Rect2D topLeft(const Point2D& p) const noexcept {
			return Rect2D(*this).topLeft(p);
		}

		/** @brief Read-only coordinates of the top left corner of the rectangle. */
		const Point2D& topLeft() const noexcept {
			return m_topLeft;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new bottom right point for the rectangle. */
		Rect2D& bottomRight(const Point2D& p) noexcept {
			m_bottomRight = p;
			return *this;
		}

		/** @brief Coordinates of the bottom right corner of the rectangle. */
		Point2D& bottomRight() noexcept {
			return m_bottomRight;
		}


		/** @brief Create a copy of the rectangle with the specified bottom right point. */
		Rect2D bottomRight(const Point2D& p) const noexcept {
			return Rect2D(*this).bottomRight(p);
		}

		/** @brief Read-only coordinates of the bottom right corner of the rectangle. */
		const Point2D& bottomRight() const noexcept {
			return m_bottomRight;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new size for the rectangle. */
		Rect2D& size(const Size2D& s) noexcept {
			m_bottomRight
				.x(topLeft().x() + s.width())
				.y(topLeft().y() + s.height());
			return *this;
		}

		/** @brief Create a copy of the rectangle with the specified size. */
		Rect2D size(const Size2D& s) const noexcept {
			return Rect2D(*this).size(s);
		}

		/** @brief Read-only sizes of the two sides of the rectangle. */
		Size2D size() const noexcept {
			return Size2D(right() - left(), bottom() - top());
		}
		/** @} */

	private:
		Point2D m_topLeft;
		Point2D m_bottomRight;
	};



	/**
	 * @brief Line in 2D space defined by two endpoints.
	 */
	class Line2D {
	public:
		/** @brief Compare two lines by endpoints. */
		friend constexpr auto operator<=>(const Line2D& lhs, const Line2D& rhs) = default;

		/** @brief Print out a line to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Line2D& line);


		/** @{ */
		Line2D() = default;

		/** @brief Construct a line from its endpoints. */
		Line2D(const Point2D& start, const Point2D& end) noexcept
			: m_start(start), m_end(end)
		{ }
		/** @} */

		/** @{ */
		Line2D(const Line2D&) = default;
		Line2D& operator=(const Line2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new starting point for the line. */
		Line2D& start(const Point2D& p) noexcept {
			m_start = p;
			return *this;
		}

		/** @brief Coordinates of the starting point of the line. */
		Point2D& start() noexcept {
			return m_start;
		}


		/** @brief Create a copy of the line with the specified starting point. */
		Line2D start(const Point2D& p) const noexcept {
			return Line2D(*this).start(p);
		}

		/** @brief Read-only coordinates of the starting point of the line. */
		const Point2D& start() const noexcept {
			return m_start;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new ending point for the line. */
		Line2D& end(const Point2D& p) noexcept {
			m_end = p;
			return *this;
		}

		/** @brief Coordinates of the ending point of the line. */
		Point2D& end() noexcept {
			return m_end;
		}


		/** @brief Create a copy of the line with the specified ending point. */
		Line2D end(const Point2D& p) const noexcept {
			return Line2D(*this).end(p);
		}

		/** @brief Read-only coordinates of the ending point of the line. */
		const Point2D& end() const noexcept {
			return m_end;
		}
		/** @} */

	private:
		Point2D m_start;
		Point2D m_end;
	};
	


	/**
	 * @brief RGBA color as four 8-bit values.
	 */
	class Color {
	public:
		/** @brief Compare two colors by components. */
		friend constexpr auto operator<=>(const Color& lhs, const Color& rhs) = default;

		/** @brief Print out a color to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Color& color);


		/** @{ */
		Color() = default;

		/** @brief Construct a new color from four RGBA components. */
		Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha) noexcept
			: m_red(red), m_green(green), m_blue(blue), m_alpha(alpha)
		{ }

		/** @brief Construct a new color from three RGB components. */
		Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue) noexcept
			: m_red(red), m_green(green), m_blue(blue)
		{ }
		/** @} */

		/** @{ */
		Color(const Color&) = default;
		Color& operator=(const Color&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new red component for the color. */
		Color& red(std::uint8_t r) noexcept {
			m_red = r;
			return *this;
		}

		/** @brief Create a copy of the color with the specified red component. */
		Color red(std::uint8_t r) const noexcept {
			return Color(*this).red(r);
		}

		/** @brief Red component of the color. */
		std::uint8_t red() const noexcept {
			return m_red;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new green component for the color. */
		Color& green(std::uint8_t g) noexcept {
			m_green = g;
			return *this;
		}

		/** @brief Create a copy of the color with the specified green component. */
		Color green(std::uint8_t g) const noexcept {
			return Color(*this).green(g);
		}

		/** @brief Green component of the color. */
		std::uint8_t green() const noexcept {
			return m_green;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new blue component for the color. */
		Color& blue(std::uint8_t b) noexcept {
			m_blue = b;
			return *this;
		}

		/** @brief Create a copy of the color with the specified blue component. */
		Color blue(std::uint8_t b) const noexcept {
			return Color(*this).blue(b);
		}

		/** @brief Blue component of the color. */
		std::uint8_t blue() const noexcept {
			return m_blue;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new alpha component for the color. */
		Color& alpha(std::uint8_t a) noexcept {
			m_alpha = a;
			return *this;
		}

		/** @brief Create a copy of the color with the specified alpha component. */
		Color alpha(std::uint8_t a) const noexcept {
			return Color(*this).alpha(a);
		}

		/** @brief Alpha component of the color. */
		std::uint8_t alpha() const noexcept {
			return m_alpha;
		}
		/** @} */

	private:
		std::uint8_t m_red = { };
		std::uint8_t m_green = { };
		std::uint8_t m_blue = { };
		std::uint8_t m_alpha = 255;
	};



	/**
	 * @brief List of observers waiting for some event to happen.
	 */
	template <typename T>
	class ObserverList {
	public:
		/** @{ */
		ObserverList() = default;
		~ObserverList() = default;
		/** @} */

		/** @{ */
		ObserverList(const ObserverList&) = default;
		ObserverList& operator=(const ObserverList&) = default;
		/** @} */

		/** @{ */
		ObserverList(ObserverList&&) = default;
		ObserverList& operator=(ObserverList&&) = default;
		/** @} */


		/** @{ */
		/** @brief Call the specified event handler on all observers. */
		template <typename... Args, typename... Params>
		void trigger(void (T::* event)(Params...), Args&&... args) const {
			for(const auto observer : m_observers) {
				(observer->*event)(std::forward<Args>(args)...);
			}
		}
		/** @} */


		/** @{ */
		/** @brief Add a new observer to the list. */
		void add(T* observer) {
			m_observers.push_back(observer);
		}

		/** @brief Remove the observer from the list. */
		void remove(T* observer) noexcept {
			const auto it = std::ranges::find(m_observers, observer);
			m_observers.erase(it);
		}
		/** @} */

	private:
		std::vector<T*> m_observers;
	};
}

#endif

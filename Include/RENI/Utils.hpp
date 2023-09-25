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
		/** @{ */
		/** @brief Compare two sizes by dimensions. */
		friend constexpr auto operator<=>(const Size2D& lhs, const Size2D& rhs) = default;

		/** @brief Print out a size to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Size2D& size);
		/** @} */


		/** @{ */
		Size2D() = default;

		/** @brief Construct a new size from width and height. */
		Size2D(int width, int height) noexcept
			: m_width(width), m_height(height)
		{ }
		/** @} */

		/** @{ */
		Size2D(const Size2D&) = default;
		Size2D& operator=(const Size2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new width for a rectangle. */
		void SetWidth(int w) noexcept {
			m_width = w;
		}

		/** @brief Width of a rectangle. */
		int GetWidth() const noexcept {
			return m_width;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new height for a rectangle. */
		void SetHeight(int h) noexcept {
			m_height = h;
		}

		/** @brief Height of a rectangle. */
		int GetHeight() const noexcept {
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
		/** @{ */
		/** @brief Compare two offsets by deltas. */
		friend constexpr auto operator<=>(const Offset2D& lhs, const Offset2D& rhs) = default;

		/** @brief Print out an offset to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Offset2D& offset);
		/** @} */


		/** @{ */
		Offset2D() = default;

		/** @brief Construct an offset from x and y deltas. */
		Offset2D(int dx, int dy) noexcept
			: m_dx(dx), m_dy(dy)
		{ }
		/** @} */

		/** @{ */
		Offset2D(const Offset2D&) = default;
		Offset2D& operator=(const Offset2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new x-delta for the offset. */
		void SetDeltaX(int dx) noexcept {
			m_dx = dx;
		}

		/** @brief Change in the x coordinate. */
		int GetDeltaX() const noexcept {
			return m_dx;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new y-delta for the offset. */
		void SetDeltaY(int dy) noexcept {
			m_dy = dy;
		}

		/** @brief Change in the y coordinate. */
		int GetDeltaY() const noexcept {
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
		/** @{ */
		/** @brief Compare two points by coordinates. */
		friend constexpr auto operator<=>(const Point2D& lhs, const Point2D& rhs) = default;

		/** @brief Print out a point to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Point2D& point);

		/** @brief Find the displacement between two points. */
		friend Offset2D operator-(const Point2D& lhs, const Point2D& rhs) noexcept {
			return Offset2D(lhs.GetX() - rhs.GetX(), lhs.GetY() - rhs.GetY());
		}

		/** @brief Move a point by the given offset. */
		friend Point2D operator+(const Point2D& p, const Offset2D& o) noexcept {
			return Point2D(p.GetX() + o.GetDeltaX(), p.GetY() + o.GetDeltaY());
		}
		/** @} */


		/** @{ */
		Point2D() = default;

		/** @brief Construct a point from x and y coordinates. */
		Point2D(int x, int y) noexcept
			: m_x(x), m_y(y)
		{ }
		/** @} */
		
		/** @{ */
		Point2D(const Point2D&) = default;
		Point2D& operator=(const Point2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new x-coordinate for the point. */
		void SetX(int x) noexcept {
			m_x = x;
		}

		/** @brief x-coordinate of the point. */
		int GetX() const noexcept {
			return m_x;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new y-coordinate for the point. */
		void SetY(int y) noexcept {
			m_y = y;
		}

		/** @brief y-coordinate of the point. */
		int GetY() const noexcept {
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
		/** @{ */
		/** @brief Compare two rectangles by points. */
		friend constexpr auto operator<=>(const Rect2D& lhs, const Rect2D& rhs) = default;

		/** @brief Print out a rectangle to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Rect2D& rect);
		/** @} */


		/** @{ */
		Rect2D() = default;

		/** @brief Construct a rectangle from its top left and bottom right points. */
		Rect2D(const Point2D& topLeft, const Point2D& bottomRight) noexcept
			: m_topLeft(topLeft), m_bottomRight(bottomRight)
		{ }

		/** @brief Construct a rectangle from its top left point and sizes of the two sides. */
		Rect2D(const Point2D& topLeft, const Size2D& size) noexcept
			: m_topLeft(topLeft) {
			SetSize(size);
		}
		/** @{ */

		/** @{ */
		Rect2D(const Rect2D&) = default;
		Rect2D& operator=(const Rect2D&) = default;
		/** @} */


		/** @{ */
		/** @brief Set the new x-coordinate for the top left corner of the rectangle. */
		void SetLeft(int l) noexcept {
			m_topLeft.SetX(l);
		}

		/** @brief x-coordinate of the top left corner of the rectangle. */
		int GetLeft() const noexcept {
			return m_topLeft.GetX();
		}
		/** @{ */


		/** @{ */
		/** @brief Set the new y-coordinate for the top left corner of the rectangle. */
		void SetTop(int t) noexcept {
			m_topLeft.SetY(t);
		}

		/** @brief y-coordinate of the top left corner of the rectangle. */
		int GetTop() const noexcept {
			return m_topLeft.GetY();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new x-coordinate for the bottom right corner of the rectangle. */
		void SetRight(int r) noexcept {
			m_bottomRight.SetX(r);
		}

		/** @brief x-coordinate of the bottom right corner of the rectangle. */
		int GetRight() const noexcept {
			return m_bottomRight.GetX();
		}
		/** @{ */


		/** @{ */
		/** @brief Set the new y-coordinate for the bottom right corner of the rectangle. */
		void SetBottom(int b) noexcept {
			m_bottomRight.SetY(b);
		}

		/** @brief y-coordinate of the bottom right corner of the rectangle. */
		int GetBottom() const noexcept {
			return m_bottomRight.GetY();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new width for the rectangle. */
		void SetWidth(int w) noexcept {
			m_bottomRight.SetX(m_topLeft.GetX() + w);
		}

		/** @brief Width of the rectangle. */
		int GetWidth() const noexcept {
			return GetRight() - GetLeft();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new height for the rectangle. */
		void SetHeight(int h) noexcept {
			m_bottomRight.SetY(m_topLeft.GetY() + h);
		}

		/** @brief Height of the rectangle. */
		int GetHeight() const noexcept {
			return GetBottom() - GetTop();
		}
		/** @} */


		/** @{ */
		/** @brief Set the new top left point for the rectangle. */
		void SetTopLeft(const Point2D& p) noexcept {
			m_topLeft = p;
		}

		/** @brief Coordinates of the top left corner of the rectangle. */
		const Point2D& GetTopLeft() const noexcept {
			return m_topLeft;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new bottom right point for the rectangle. */
		void SetBottomRight(const Point2D& p) noexcept {
			m_bottomRight = p;
		}

		/** @brief Coordinates of the bottom right corner of the rectangle. */
		const Point2D& GetBottomRight() const noexcept {
			return m_bottomRight;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new size for the rectangle. */
		void SetSize(const Size2D& s) noexcept {
			m_bottomRight.SetX(m_topLeft.GetX() + s.GetWidth());
			m_bottomRight.SetY(m_topLeft.GetY() + s.GetHeight());
		}

		/** @brief Size of the two sides of the rectangle. */
		Size2D GetSize() const noexcept {
			return Size2D(GetRight() - GetLeft(), GetBottom() - GetTop());
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
		/** @{ */
		/** @brief Compare two lines by endpoints. */
		friend constexpr auto operator<=>(const Line2D& lhs, const Line2D& rhs) = default;

		/** @brief Print out a line to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Line2D& line);
		/** @} */


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
		void SetStart(const Point2D& p) noexcept {
			m_start = p;
		}

		/** @brief Coordinates of the starting point of the line. */
		const Point2D& GetStart() const noexcept {
			return m_start;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new ending point for the line. */
		void SetEnd(const Point2D& p) noexcept {
			m_end = p;
		}

		/** @brief Coordinates of the ending point of the line. */
		const Point2D& GetEnd() const noexcept {
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
		/** @{ */
		/** @brief Compare two colors by components. */
		friend constexpr auto operator<=>(const Color& lhs, const Color& rhs) = default;

		/** @brief Print out a color to a @c std::ostream. */
		friend std::ostream& operator<<(std::ostream& out, const Color& color);
		/** @} */


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
		void SetRed(std::uint8_t r) noexcept {
			m_red = r;
		}

		/** @brief Red component of the color. */
		std::uint8_t GetRed() const noexcept {
			return m_red;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new green component for the color. */
		void SetGreen(std::uint8_t g) noexcept {
			m_green = g;
		}

		/** @brief Green component of the color. */
		std::uint8_t GetGreen() const noexcept {
			return m_green;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new blue component for the color. */
		void SetBlue(std::uint8_t b) noexcept {
			m_blue = b;
		}

		/** @brief Blue component of the color. */
		std::uint8_t GetBlue() const noexcept {
			return m_blue;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new alpha component for the color. */
		void SetAlpha(std::uint8_t a) noexcept {
			m_alpha = a;
		}

		/** @brief Alpha component of the color. */
		std::uint8_t GetAlpha() const noexcept {
			return m_alpha;
		}
		/** @} */

	private:
		std::uint8_t m_red = { };
		std::uint8_t m_green = { };
		std::uint8_t m_blue = { };
		std::uint8_t m_alpha = 255;
	};
}

#endif

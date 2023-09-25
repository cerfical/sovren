#ifndef RENI_RENDER_HEADER
#define RENI_RENDER_HEADER

#include "Utils.hpp"

namespace RENI {
	/**
	 * @brief 3D/2D renderer.
	 */
	class Render {
	public:
		/** @{ */
		/** @brief Start the rendering process. */
		virtual void start() = 0;
		/** @brief End the rendering process. */
		virtual void end() = 0;
		/** @} */

		/** @{ */
		/** @brief Draw a line. */
		virtual void drawLine(const Line2D& line) = 0;

		/** @brief Draw the outline of a rectangle. */
		virtual void drawRect(const Rect2D& rect) = 0;
		/** @brief Draw the interior of a rectangle. */
		virtual void fillRect(const Rect2D& rect) = 0;

		/** @brief Set the color to be used for drawing operations. */
		virtual void setDrawColor(Color color) = 0;
		/** @brief Fill the Window with the specified color. */
		virtual void clear(Color color) = 0;
		/** @} */

	protected:
		/** @{ */
		~Render() = default;
		/** @} */
	};
}

#endif
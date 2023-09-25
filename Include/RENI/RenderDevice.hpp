#ifndef RENI_RENDER_DEVICE_HEADER
#define RENI_RENDER_DEVICE_HEADER

#include "Utils.hpp"

namespace RENI {
	/**
	 * @brief Two-dimensional surface for performing 2D/3D rendering operations.
	 */
	class RenderDevice {
	public:
		/** @{ */
		/** Destroy the device. */
		virtual ~RenderDevice() = default;
		/** @} */


		/** @{ */
		/** @brief Start a new drawing sequence. */
		virtual void startDraw() = 0;

		/** @brief End the drawing sequence. */
		virtual void endDraw() = 0;
		/** @} */


		/** @{ */
		/** @brief Set the new size of the buffers backing the device. */
		virtual void resize(const Size2D& newSize) = 0;

		/** @brief Set the color to be used for drawing operations. */
		virtual void setDrawColor(Color c) = 0;
		/** @} */


		/** @{ */
		/** @brief Draw a line. */
		virtual void drawLine(const Line2D& l) = 0;

		/** @brief Draw the outline of a rectangle. */
		virtual void drawRect(const Rect2D& r) = 0;

		/** @brief Draw the interior of a rectangle. */
		virtual void fillRect(const Rect2D& r) = 0;

		/** @brief Fill the window with the specified color. */
		virtual void clear(Color c) = 0;
		/** @} */
	};
}

#endif
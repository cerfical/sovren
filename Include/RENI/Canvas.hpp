#ifndef RENI_CANVAS_HEADER
#define RENI_CANVAS_HEADER

#include "Utils.hpp"
#include <memory>

namespace RENI {
	/**
	 * @brief Interface for performing drawing operations on a Canvas.
	 */
	class DrawingContext {
	public:
		/** @{ */
		/** @brief Destroy the DrawingContext. */
		virtual ~DrawingContext() = default;
		/** @} */

		/** @{ */
		DrawingContext(const DrawingContext&) = delete;
		DrawingContext& operator=(const DrawingContext&) = delete;
		/** @} */

		/** @{ */
		DrawingContext(DrawingContext&&) = delete;
		DrawingContext& operator=(DrawingContext&&) = delete;
		/** @} */

		/** @{ */
		/** @brief Draw a line. */
		virtual void DrawLine(const Line2D& line) = 0;

		/** @brief Draw the outline of a rectangle. */
		virtual void DrawRect(const Rect2D& rect) = 0;
		/** @brief Draw the interior of a rectangle. */
		virtual void FillRect(const Rect2D& rect) = 0;

		/** @brief Set the color to be used for drawing operations. */
		virtual void SetDrawColor(Color color) = 0;
		/** @brief Fill the Canvas with the specified Color. */
		virtual void Clear(Color color) = 0;
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new DrawingContext. */
		DrawingContext() = default;
		/** @} */
	};

	/**
	 * @brief Drawing surface of a Window.
	 */
	class Canvas {
	public:
		/** @{ */
		/** @brief Destroy the Canvas. */
		virtual ~Canvas() = default;
		/** @} */

		/** @{ */
		Canvas(const Canvas&) = delete;
		Canvas& operator=(const Canvas&) = delete;
		/** @} */

		/** @{ */
		Canvas(Canvas&&) = delete;
		Canvas& operator=(Canvas&&) = delete;
		/** @} */

		/** @{ */
		/** @brief Start a drawing sequence. */
		virtual std::unique_ptr<DrawingContext> BeginDraw() = 0;

		/** @brief Set a new size of the Canvas. */
		virtual void Resize(const Extent2D& size) = 0;
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new Canvas. */
		Canvas() = default;
		/** @} */
	};
}

#endif
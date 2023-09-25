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
		/** @brief Draw a rectangle on the Canvas with the specified border Color. */
		virtual void DrawRect(const Rect& rect, Color color) = 0;
		/** @brief Fill a rectangle on the Canvas with specified Color. */
		virtual void FillRect(const Rect& rect, Color color) = 0;

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
		virtual void Resize(Extent2D size) = 0;
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new Canvas. */
		Canvas() = default;
		/** @} */
	};
}

#endif
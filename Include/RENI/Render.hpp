#ifndef RENI_RENDER_HEADER
#define RENI_RENDER_HEADER

#include "Utils.hpp"

namespace RENI {
	class Window;
	/**
	 * @brief Simple renderer for rendering 2D/3D graphics.
	 */
	class Render {
		struct Impl;

	public:
		/**
		 * @brief Represents the context for performing a single render sequence.
		 */
		class Context {
			friend Render;

		public:
			/** @{ */
			~Context() {
				Present();
			}
			/** @} */


			/** @{ */
			Context(const Context&) = delete;
			Context& operator=(const Context&) = delete;
			/** @} */

			/** @{ */
			Context(Context&&) = delete;
			Context& operator=(Context&&) = delete;
			/** @} */


			/** @{ */
			/** @brief Set the color to be used for drawing operations. */
			Context& SetDrawColor(Color c);

			/** @brief Fill the window with the specified color. */
			Context& Clear(Color c);
			/** @} */


			/** @{ */
			/** @brief Draw a line. */
			Context& DrawLine(const Line2D& l);

			/** @brief Draw the outline of a rectangle. */
			Context& DrawRect(const Rect2D& r);
			
			/** @brief Draw the interior of a rectangle. */
			Context& FillRect(const Rect2D& r);
			/** @} */


			/** @{ */
			/** @brief End the render sequence and display the result in the window. */
			void Present();
			/** @} */

		private:
			/** @{ */
			explicit Context(Impl& impl) noexcept
				: m_impl(&impl)
			{ }
			/** @} */

			Impl* m_impl = nullptr;
			bool m_presented = false;
		};

		/** @{ */
		/** @brief Construct a new renderer attached to the window. */
		explicit Render(Window& window);
		
		/** @brief Destroy the renderer. */
		~Render();
		/** @} */

		/** @{ */
		Render(Render&&) = default;
		Render& operator=(Render&&) = default;
		/** @} */

		/** @{ */
		Render(const Render&) = delete;
		Render& operator=(const Render&) = delete;
		/** @} */

		/** @{ */
		/** @brief Start the rendering process. */
		Context StartRender();
		/** @} */

	private:
		std::unique_ptr<Impl> m_impl;
	};
}

#endif
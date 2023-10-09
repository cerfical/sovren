#ifndef RENI_RENDER_WINDOW_HEADER
#define RENI_RENDER_WINDOW_HEADER

#include "RenderThread.hpp"
#include "RenderDevice.hpp"
#include "Window.hpp"

namespace RENI {
	/**
	 * @brief Extension of the basic Window to simplify visualizing simple graphics.
	 */
	class RenderWindow : public Window {
		friend RenderThread;

	public:
		/** @{ */
		/** @brief Construct a new window with default settings. */
		RenderWindow();

		/** @brief Destroy the window. */
		~RenderWindow() override = default;
		/** @} */


		/** @{ */
		RenderWindow(RenderWindow&&) = default;
		RenderWindow& operator=(RenderWindow&&) = default;
		/** @} */

		/** @{ */
		RenderWindow(const RenderWindow&) = delete;
		RenderWindow& operator=(const RenderWindow&) = delete;
		/** @} */


		/** @{ */
		/** @brief Render device capable of rendering to the window. */
		RenderDevice* renderDevice();

		/** @brief Background thread rendering the window. */
		RenderThread* renderThread();
		/** @} */


	protected:
		/** @{ */
		/** @brief Called when the window needs to render itself on the screen. */
		virtual void onRender() { }
		/** @} */

		/** @{ */
		void onResize(const Size2D& newSize, const Size2D& oldSize) override;
		/** @} */


	private:
		/** @{ */
		std::unique_ptr<RenderDevice> m_renderDevice;
		RenderThread m_renderThread;
		/** @} */
	};
}

#endif
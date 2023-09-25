#ifndef RENI_WINDOW_HANDLE_HEADER
#define RENI_WINDOW_HANDLE_HEADER

#include "Utils.hpp"

#include <string_view>
#include <string>

namespace RENI {
	class Window;
	/**
	 * @brief Abstraction for managing platform-specific window implementations.
	 */
	class WindowHandle {
	public:
		/** @{ */
		virtual ~WindowHandle() = default;
		/** @} */


		/** @{ */
		/** @brief Set the new client area dimensions for the window. */
		virtual void setClientSize(const Size2D& size) = 0;

		/** @brief Dimensions of the window's client area. */
		virtual Size2D clientSize() const = 0;


		/** @brief Set the new title for the window. */
		virtual void setTitle(std::string_view title) = 0;
		
		/** @brief Window's title bar text. */
		virtual std::string title() const = 0;


		/** @brief Change the visibility of the window. */
		virtual void setVisible(bool visible) = 0;
		
		/** @brief Check if the window is currently visible. */
		virtual bool visible() const = 0;
		/** @} */


		/** @{ */
		/** @brief Enable or disable mouse capture for the window. */
		virtual void setMouseCapture(bool enable) = 0;

		/** @brief Current mouse cursor position relative to the window. */
		virtual Point2D mousePos() const = 0;
		/** @} */


		/** @{ */
		/** @brief Platform-native handle of the window. */
		virtual void* nativeHandle() const = 0;
		/** @} */


		/** @{ */
		/** @brief Attach a Window object to the handle. */
		void attachWindow(Window* window) noexcept {
			m_window = window;
		}

		/** @brief Detach the Window object from the handle. */
		void detachWindow() noexcept {
			attachWindow(nullptr);
		}
		
		/** @brief Window object housing the handle. */
		Window* attachedWindow() const noexcept {
			return m_window;
		}
		/** @} */

	private:
		Window* m_window = nullptr;
	};
}

#endif
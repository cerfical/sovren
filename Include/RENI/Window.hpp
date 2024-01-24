#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#include "Size2D.hpp"
#include "Point2D.hpp"

#include "Keys.hpp"
#include "MouseButtons.hpp"

#include <memory>
#include <string_view>
#include <string>

namespace RENI {
	/**
	 * @brief Simple rectangular window on the screen.
	 */
	class Window {
	public:
		/** @{ */
		/** @brief Construct a new window with default settings. */
		Window();

		/** @brief Destroy the window. */
		virtual ~Window();
		/** @} */


		/** @{ */
		Window(Window&&) = default;
		Window& operator=(Window&&) = default;
		/** @} */

		/** @{ */
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		/** @} */


		/** @{ */
		/** @brief Set the new client area dimensions for the window. */
		void setSize(Size2D s);
		
		/** @brief Get the dimensions of the window's client area. */
		Size2D size() const;

		/** @brief Get the window's width. */
		int width() const {
			return size().width;
		}

		/** @brief Get the window's height. */
		int height() const {
			return size().height;
		}
		/** @} */


		/** @{ */
		/** @brief Set the new title for the window. */
		void setTitle(std::string_view title);

		/** @brief Get window's title text. */
		std::string title() const;
		/** @} */


		/** @{ */
		/** @brief Change the visibility of the window. */
		void setVisible(bool visible);

		/** @brief Make the window visible on the screen. */
		void show() {
			setVisible(true);
		}

		/** @brief Hide the window. */
		void hide() {
			setVisible(false);
		}

		/** @brief Check if the window is currently visible. */
		bool visible() const;
		/** @} */

		
		/** @{ */
		/** @brief Pressed/released state of a key for the window. */
		bool keyState(Keys key) const;

		/** @brief Pressed/released state of a mouse button for the window. */
		bool buttonState(MouseButtons button) const;

		/** @brief Enables or disables mouse input capture by this window. */
		void toggleMouseCapture();
		/** @} */


		/** @{ */
		/** @brief Get the platform-native handle for the window. */
		void* nativeHandle() const;
		/** @} */


	protected:
		/** @{ */
		/** @brief Called when the window has been closed. */
		virtual void onClose();

		/** @brief Called when the window has been resized. */
		virtual void onResize(Size2D newSize, Size2D oldSize) { }
		/** @} */


		/** @{ */
		/** @brief Called when a key has been pressed. */
		virtual void onKeyPress(Keys key) { }
		
		/** @brief Called when a key has been released. */
		virtual void onKeyRelease(Keys key) { }
		/** @} */


		/** @{ */
		/** @brief Called when a mouse button has been pressed. */
		virtual void onMousePress(MouseButtons button) { }

		/** @brief Called when a mouse button has been released. */
		virtual void onMouseRelease(MouseButtons button) { }

		/** @brief Called when the mouse has been moved. */
		virtual void onMouseMove(Point2D newPos, Point2D oldPos) { }
		/** @} */


	private:
		/** @{ */
		struct ImplBase; struct Impl;
		std::unique_ptr<Impl> m_impl;
		/** @} */
	};
}

#endif
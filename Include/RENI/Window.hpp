#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#include "Utils.hpp"

#include "KeysState.hpp"
#include "MouseState.hpp"

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
		void Resize(const Size2D& size);
		
		/** @brief Get the dimensions of the window's client area. */
		const Size2D& GetSize() const;


		/** @brief Set the new title for the window. */
		void SetTitle(std::string_view title);

		/** @brief Get window's title text. */
		std::string GetTitle() const;


		/** @brief Change the visibility of the window. */
		void SetVisible(bool visible);

		/** @brief Check if the window is currently visible. */
		bool IsVisible() const;
		/** @} */

		
		/** @{ */
		/** @brief Get keyboard input state for the window. */
		const KeysState& GetKeysState() const noexcept;

		/** @brief Get mouse input state for the window. */
		const MouseState& GetMouseState() const noexcept;

		/** @brief Enables or disables mouse input capture by this window. */
		void ToggleMouseCapture();
		/** @} */


	protected:
		/** @{ */
		/** @brief Called when the window has been closed. */
		virtual void OnClose();

		/** @brief Called when the window has been resized. */
		virtual void OnResize(const Size2D& newSize, const Size2D& oldSize) { }
		/** @} */


		/** @{ */
		/** @brief Called when a key has been pressed. */
		virtual void OnKeyPress(Keys pressedKey) { }
		
		/** @brief Called when a key has been released. */
		virtual void OnKeyRelease(Keys releasedKey) { }
		/** @} */


		/** @{ */
		/** @brief Called when a mouse button has been pressed. */
		virtual void OnMousePress(MouseButtons pressedButton) { }

		/** @brief Called when a mouse button has been released. */
		virtual void OnMouseRelease(MouseButtons releasedButton) { }

		/** @brief Called when the mouse has been moved. */
		virtual void OnMouseMove(const Point2D& newPos, const Point2D& oldPos) { }
		/** @} */


	private:
		struct Impl; std::unique_ptr<Impl> m_impl;
	};
}

#endif
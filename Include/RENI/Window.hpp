#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#include "Canvas.hpp"
#include "MouseState.hpp"
#include "KeysState.hpp"

#include <string>
#include <string_view>
#include <memory>

namespace RENI {
	/**
	 * @brief Interface for handling window events.
	 */
	class WindowObserver {
	public:
		/** @{ */
		/** @brief Called when a window has become visible. */
		virtual void OnWindowShow() { }

		/** @brief Called when a window has been made hidden. */
		virtual void OnWindowHide() { }

		/** @brief Called when a window has been closed. */
		virtual void OnWindowClose() { }

		/** @brief Called when a window has been resized. */
		virtual void OnWindowResize() { }

		/** @brief Called when a window needs to draw itself. */
		virtual void OnWindowDraw() { }
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new WindowObserver. */
		WindowObserver() = default;

		/** @brief Destroy the WindowObserver. */
		~WindowObserver() = default;
		/** @} */
	};

	/**
	 * @brief Simple rectangular window on the screen.
	 */
	class Window {
	public:
		/** @{ */
		/** @brief Construct a new Window with default settings. */
		Window() noexcept;

		/** @brief Destroy the Window. */
		~Window();
		/** @} */


		/** @{ */
		/** @brief Construct a new Window by moving from another Window. */
		Window(Window&&) noexcept;

		/** @brief Move another Window into this Window. */
		Window& operator=(Window&&) noexcept;
		/** @} */


		/** @{ */
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		/** @} */


		/** @{ */
		/** @brief Set new client area dimensions for the window. */
		void SetClientArea(Extent2D clientArea);
		
		/** @brief Get the dimensions of the window's client area. */
		Extent2D GetClientArea() const;


		/** @brief Set a new title for the window. */
		void SetTitle(std::string_view title);

		/** @brief Get the window's title. */
		std::string GetTitle() const;


		/** @brief Change the visibility of the window. */
		void SetVisible(bool visible);

		/** @brief Check if the window is currently visible. */
		bool IsVisible() const;
		/** @} */


		/** @{ */
		/** @brief Register an EventObserver to receive the window events. */
		void AddObserver(WindowObserver& observer) const;
		
		/** @brief Unregister an EventObserver from receiving the window events. */
		void RemoveObserver(WindowObserver& observer) const;


		/** @brief Get the mouse input state for this window. */
		const MouseState& GetMouseState() const;

		/** @brief Get the keyboard input state for this window. */
		const KeysState& GetKeysState() const;


		/** @brief Cause all mouse input to be intercepted by this window. */
		void SetMouseCapture();

		/** @brief Release the mouse capture from this window. */
		void ReleaseMouseCapture();
		/** @} */


		/** @{ */
		/** @brief Get the Canvas associated with the window. */
		Canvas& GetCanvas();
		/** @} */

	private:
		/** @brief System dependent implementation details. */
		class Impl;

		/** @{ */
		/** @brief Get a constant reference to the implementation, initializing it if necessary. */
		const Impl& GetImpl() const;
		/** @brief Get a reference to the implementation, initializing it if necessary. */
		Impl& GetImpl();
		/** @} */

		mutable std::unique_ptr<Impl> impl;
	};
}

#endif
#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#include "Canvas.hpp"
#include "EventObserver.hpp"
#include "MouseInput.hpp"
#include "KeyInput.hpp"
#include "Utils.hpp"

#include <string>
#include <string_view>
#include <functional>
#include <memory>

namespace RENI {
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
		/** @brief Move another Window into the Window. */
		Window& operator=(Window&&) noexcept;
		/** @} */

		/** @{ */
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		/** @} */

		/** @{ */
		/** @brief Set new client area dimensions for the Window. */
		void SetClientArea(Extent2D clientArea);
		/** @brief Get the dimensions of the Window's client area. */
		Extent2D GetClientArea() const;

		/** @brief Set a new title for the Window. */
		void SetTitle(std::string_view title);
		/** @brief Get the Window's title. */
		std::string GetTitle() const;

		/** @brief Change the visibility of the Window. */
		void SetVisible(bool visible);
		/** @brief Check if the Window is currently visible. */
		bool IsVisible() const;
		/** @} */

		/** @{ */
		/** @brief Register the EventObserver to receive the Window events. */
		void RegisterObserver(EventObserver& observer);
		/** @brief Unregister the EventObserver from receiving the Window events. */
		void UnregisterObserver(EventObserver& observer);

		/** @brief Register a new callback to be called when the event of interest occurs. */
		void NotifyWhen(EventTypes event, std::function<void()> func);

		/** @brief Get the mouse input state for the Window. */
		const MouseInput& GetMouseState() const;
		/** @brief Get the keyboard input state for the Window. */
		const KeyInput& GetKeysState() const;

		/** @brief Cause all mouse input to be intercepted by this Window. */
		void SetMouseCapture();
		/** @brief Release the mouse capture from this Window, effectively canceling the effect of SetMouseCapture(). */
		void ReleaseMouseCapture();
		/** @} */

		/** @{ */
		/** @brief Get the Canvas associated with the Window. */
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
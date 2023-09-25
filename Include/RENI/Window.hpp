#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#include "Canvas.hpp"
#include "Utils.hpp"

#include <string>
#include <string_view>
#include <functional>
#include <memory>

namespace RENI {
	/**
	 * @brief Types of the events that a Window can generate.
	 */
	enum class WindowEvent {
		Close,
		Hide,
		Show,
		Resize,
		Draw
	};

	/**
	 * @brief Simple rectangular window on the screen.
	 */
	class Window {
	public:
		/** @{ */
		/** @brief Construct a new Window with default settings. */
		Window() noexcept = default;

		/** @brief Construct a new Window with a title and of the specified size. */
		Window(std::string_view title, Extent2D size);

		/** @brief Destroy the Window. */
		~Window();
		/** @} */

		/** @{ */
		/** @brief Construct a new Window by moving from another Window. */
		Window(Window&&) = default;

		/** @brief Move another Window into the Window. */
		Window& operator=(Window&&) = default;
		/** @} */

		/** @{ */
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		/** @} */

		/** @{ */
		/** @brief Register a new callback for the event of interest. */
		void NotifyWhen(WindowEvent event, std::function<void()> func);

		/** @brief Call every callback associated with an event. */
		void TriggerEvent(WindowEvent event);
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
		/** @brief Get the Canvas associated with the Window. */
		Canvas& GetCanvas();
		/** @} */

	private:
		class Impl;

		const Impl& GetImpl() const;
		Impl& GetImpl();

		mutable std::unique_ptr<Impl> impl;
	};
}

#endif
#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#include "WindowHandle.hpp"
#include "Events.hpp"

#include <memory>
#include <string_view>
#include <string>

namespace RENI {
	/**
	 * @brief Interface for monitoring window events.
	 */
	class WindowObserver {
	public:
		/** @{ */
		/** @brief Called when the window is closed. */
		virtual void onClose(const CloseEvent& e) { }

		/** @brief Called when the window is resized. */
		virtual void onResize(const ResizeEvent& e) { }
		/** @} */

	protected:
		/** @{ */
		~WindowObserver() = default;
		/** @} */
	};



	/**
	 * @brief Simple rectangular window on the screen.
	 */
	class Window : protected WindowObserver, protected KeysObserver, protected MouseObserver {
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
		void setClientSize(const Size2D& size);
		
		/** @brief Dimensions of the window's client area. */
		const Size2D& clientSize() const noexcept {
			return m_clientSize;
		}


		/** @brief Set the new title for the window. */
		void setTitle(std::string_view title) {
			handle()->setTitle(title);
		}

		/** @brief Window's title bar text. */
		std::string title() const {
			return handle()->title();
		}


		/** @brief Change the visibility of the window. */
		void setVisible(bool visible) {
			handle()->setVisible(visible);
		}

		/** @brief Check if the window is currently visible. */
		bool visible() const noexcept {
			return m_visible;
		}
		/** @} */


		/** @{ */
		/** @brief Register a MouseObserver to receive updates to the window's mouse state. */
		void addMouseObserver(MouseObserver* o) {
			m_mouseObservers.add(o);
		}

		/** @brief Unregister a MouseObserver from receiving updates. */
		void removeMouseObserver(MouseObserver* o) {
			m_mouseObservers.remove(o);
		}

		/** @brief Cause all mouse input to be intercepted by this window. */
		void captureMouse() {
			handle()->setMouseCapture(true);
		}

		/** @brief Release the mouse capture from this window. */
		void releaseMouse() {
			handle()->setMouseCapture(false);
		}

		/** @brief Current mouse input state for the window. */
		const MouseState& mouse() const noexcept {
			return m_mouse;
		}
		/** @} */
		
		
		/** @{ */
		/** @brief Register a KeysObserver to receive updates to the window's keyboard state. */
		void addKeysObserver(KeysObserver* o) {
			m_keysObservers.add(o);
		}

		/** @brief Unregister a KeysObserver from receiving updates. */
		void removeKeysObserver(KeysObserver* o) {
			m_keysObservers.remove(o);
		}

		/** @brief Current keyboard input state for the window. */
		const KeysState& keys() const noexcept {
			return m_keys;
		}
		/** @} */


		/** @{ */
		/** @brief Register a WindowObserver to receive updates to the window's state. */
		void addWindowObserver(WindowObserver* o) {
			m_windowObservers.add(o);
		}

		/** @brief Unregister a WindowObserver from receiving updates. */
		void removeWindowObserver(WindowObserver* o) {
			m_windowObservers.remove(o);
		}

		/** @brief Trigger a new event in the window context. */
		void triggerEvent(const Event& event);

		/** @brief Read access to the WindowHandle object backing the window. */
		const WindowHandle* handle() const noexcept {
			return m_handle.get();
		}

		/** @brief Write access to the WindowHandle object backing the window. */
		WindowHandle* handle() noexcept {
			return m_handle.get();
		}
		/** @} */

	protected:
		/** @{ */
		/** @brief Called for every event triggered on the window. */
		virtual void onEvent(const Event& e);
		/** @} */

	private:
		std::unique_ptr<WindowHandle> m_handle;

		ObserverList<WindowObserver> m_windowObservers;
		ObserverList<MouseObserver> m_mouseObservers;
		ObserverList<KeysObserver> m_keysObservers;

		MouseState m_mouse;
		KeysState m_keys;

		Size2D m_clientSize;
		bool m_visible = false;
	};
}

#endif
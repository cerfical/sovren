#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#define RENI_KEY_LIST \
	RENI_KEY(None) /**< @brief Empty default value. */ \
	RENI_KEY(LeftArrow) /**< @brief Left arrow key. */ \
	RENI_KEY(RightArrow) /**< @brief Right arrow key. */ \
	RENI_KEY(UpArrow) /**< @brief Up arrow key. */ \
	RENI_KEY(DownArrow) /**< @brief Down arrow key. */

#define RENI_MOUSE_BUTTON_LIST \
	RENI_MOUSE_BUTTON(None) /**< @brief Empty default value. */ \
	RENI_MOUSE_BUTTON(Left) /**< @brief Left mouse button. */ \
	RENI_MOUSE_BUTTON(Middle) /**< @brief Middle mouse button. */ \
	RENI_MOUSE_BUTTON(Right) /**< @brief Right mouse button. */

#include "Canvas.hpp"

#include <string>
#include <string_view>

namespace RENI {
#define RENI_KEY(key) key,
	/**
	 * @brief Identifies available keys on the keyboard.
	 */
	enum class Keys {
		RENI_KEY_LIST
	};
#undef RENI_KEY

	/** @brief Print a Keys enumerator to a @c std::ostream. */
	std::ostream& operator<<(std::ostream& out, Keys key);



#define RENI_MOUSE_BUTTON(button) button,
	/**
	 * @brief Identifies available buttons on the mouse.
	 */
	enum class MouseButtons {
		RENI_MOUSE_BUTTON_LIST
	};
#undef RENI_MOUSE_BUTTON

	/** @brief Print a MouseButtons enumerator to a @c std::ostream. */
	std::ostream& operator<<(std::ostream& out, MouseButtons button);



	/**
	 * @brief Simple rectangular window on the screen.
	 */
	class Window {
		class Impl;

	public:
		/**
		 * @brief Contains information about the current state of the mouse.
		 */
		class MouseState {
			friend Window;

		public:
			/** @{ */
			/** @brief Button, whose pressed/released state was changed recently. */
			MouseButtons GetActiveButton() const noexcept {
				return m_activeButton;
			}
			/** @} */

			/** @{ */
			/** @brief Check if a button is pressed. */
			bool IsButtonPressed(MouseButtons button) const noexcept;

			/** @brief Check if a button is released. */
			bool IsButtonReleased(MouseButtons button) const noexcept {
				return !IsButtonPressed(button);
			}

			/** @brief Check if any button is pressed. */
			bool IsAnyButtonPressed() const noexcept {
				return !m_pressedButtons.empty();
			}
			/** @} */


			/** @{ */
			/** @brief Previous cursor position of the mouse. */
			const Point2D& GetOldCursorPos() const noexcept {
				return m_oldCursorPos;
			}

			/** @brief Current cursor position of the mouse. */
			const Point2D& GetCursorPos() const noexcept {
				return m_cursorPos;
			}

			/** @brief Last mouse cursor displacement. */
			Displace2D GetCursorDisplace() const noexcept {
				return {
					.dx = m_cursorPos.x - m_oldCursorPos.x,
					.dy = m_cursorPos.y - m_oldCursorPos.y
				};
			}
			/** @} */

		private:
			/** @{ */
			MouseState() = default;
			~MouseState() = default;
			/** @} */

			/** @{ */
			MouseState(MouseState&&) = default;
			MouseState& operator=(MouseState&&) = default;
			/** @} */

			/** @{ */
			MouseState(const MouseState&) = default;
			MouseState& operator=(const MouseState&) = default;
			/** @} */

			/** @{ */
			void PressButton(Window& window, MouseButtons button);
			void ReleaseButton(Window& window, MouseButtons button);
			
			void UpdateCursorPos(Window& window, const Point2D& cursorPos);
			void SetCursorPos(const Point2D& cursorPos) noexcept {
				m_cursorPos = m_oldCursorPos = cursorPos;
			}
			/** @} */

			std::vector<MouseButtons> m_pressedButtons;

			Point2D m_oldCursorPos = { }, m_cursorPos = { };
			MouseButtons m_activeButton = { };
		};
		

		/**
		 * @brief Contains information about the current state of the keys on the keyboard.
		 */
		class KeysState {
			friend Window;

		public:
			/** @{ */
			/** @brief Key, whose pressed/released state was changed recently. */
			Keys GetActiveKey() const noexcept {
				return m_activeKey;
			}
			/** @} */

			/** @{ */
			/** @brief Check if a key is pressed. */
			bool IsKeyPressed(Keys key) const noexcept;

			/** @brief Check if a key is released. */
			bool IsKeyReleased(Keys key) const noexcept {
				return !IsKeyPressed(key);
			}

			/** @brief Check if any key is pressed. */
			bool IsAnyKeyPressed() const noexcept {
				return !m_pressedKeys.empty();
			}
			/** @} */

		private:			
			/** @{ */
			KeysState() = default;
			~KeysState() = default;
			/** @} */

			/** @{ */
			KeysState(KeysState&&) = default;
			KeysState& operator=(KeysState&&) = default;
			/** @} */

			/** @{ */
			KeysState(const KeysState&) = default;
			KeysState& operator=(const KeysState&) = default;
			/** @} */

			/** @{ */
			void PressKey(Window& window, Keys key);
			void ReleaseKey(Window& window, Keys key);
			/** @} */

			std::vector<Keys> m_pressedKeys;
			Keys m_activeKey = { };
		};

		
		/** @{ */
		/** @brief Construct a new window with default settings. */
		Window();

		/** @brief Destroy the window. */
		virtual ~Window() = default;
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
		/** @brief Set new client area dimensions for the window. */
		void SetClientArea(const Extent2D& clientArea);
		
		/** @brief Get the dimensions of the window's client area. */
		const Extent2D& GetClientArea() const noexcept {
			return m_clientArea;
		}


		/** @brief Set a new title for the window. */
		void SetTitle(std::string_view title);

		/** @brief Get the window's title. */
		std::string GetTitle() const;


		/** @brief Change the visibility of the window. */
		void SetVisible(bool visible);

		/** @brief Check if the window is currently visible. */
		bool IsVisible() const noexcept {
			return m_visible;
		}
		/** @} */


		/** @{ */
		/** @brief Cause all mouse input to be intercepted by this window. */
		void CaptureMouse();

		/** @brief Release the mouse capture from this window. */
		void ReleaseMouse();
		/** @} */


		/** @{ */
		/** @brief Get the Canvas associated with the window. */
		Canvas* GetCanvas();
		/** @} */


		MouseState mouse; /**< @brief Mouse input configuration for the window. */
		KeysState keys; /**< @brief Keyboard input configuration for the window. */


	protected:
		/** @{ */
		/** @brief Called when a window becomes visible. */
		virtual void OnShow() { }

		/** @brief Called when a window becomes hidden. */
		virtual void OnHide() { }

		/** @brief Called when a window is closed. */
		virtual void OnClose() { }

		/** @brief Called when a window is resized. */
		virtual void OnResize() { }

		/** @brief Called when a window needs to draw itself. */
		virtual void OnDraw() { }
		/** @} */


		/** @{ */
		/** @brief Called when a mouse button is released. */
		virtual void OnButtonRelease() { }

		/** @brief Called when a mouse button is pressed. */
		virtual void OnButtonPress() { }

		/** @brief Called when the mouse is moved. */
		virtual void OnMouseMove() { }
		/** @} */


		/** @{ */
		/** @brief Called when a key is pressed. */
		virtual void OnKeyPress() { }

		/** @brief Called when a key is released. */
		virtual void OnKeyRelease() { }
		/** @} */

	private:
		/** @{ */
		void UpdateClientArea(const Extent2D& clientArea);
		void UpdateVisibility(bool visible);
		/** @} */

		ImplHolder<Impl> m_impl;

		Extent2D m_clientArea = { };
		bool m_visible = false;
	};
}

#endif
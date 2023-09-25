#ifndef RENI_MOUSE_BUTTONS_HEADER
#define RENI_MOUSE_BUTTONS_HEADER

#define RENI_MOUSE_BUTTON_LIST \
	RENI_MOUSE_BUTTON(None) /**< @brief Empty default value. */ \
	RENI_MOUSE_BUTTON(Left) /**< @brief Left mouse button. */ \
	RENI_MOUSE_BUTTON(Middle) /**< @brief Middle mouse button. */ \
	RENI_MOUSE_BUTTON(Right) /**< @brief Right mouse button. */

#include "Utils.hpp"

#include <ostream>
#include <vector>

namespace RENI {

#define RENI_MOUSE_BUTTON(b) b,
	
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
	 * @brief Contains information about the input state of the mouse.
	 */
	class MouseState {
	public:
		/** @{ */
		MouseState() = default;
		~MouseState() = default;
		/** @} */

		/** @{ */
		MouseState(MouseState&&) = default;
		MouseState& operator=(MouseState&&) = default;
		/** @} */

		/** @{ */
		MouseState(const MouseState&) = delete;
		MouseState& operator=(const MouseState&) = delete;
		/** @} */


		/** @{ */
		/** @brief Get the pressed/released state of a mouse button. */
		bool operator[](MouseButtons button) const noexcept {
			return isButtonPressed(button);
		}
		/** @} */
		
		
		/** @{ */
		/** @brief Check if a button is pressed. */
		bool isButtonPressed(MouseButtons b) const noexcept;

		/** @brief Check if a button is released. */
		bool isButtonReleased(MouseButtons b) const noexcept {
			return !isButtonPressed(b);
		}

		/** @brief Check if any button is pressed. */
		bool anyButtonPressed() const noexcept {
			return !m_pressedButtons.empty();
		}
		/** @} */


		/** @{ */
		/** @brief Toggle a button to the pressed state. */
		void pressButton(MouseButtons b);

		/** @brief Toggle a button to the released state. */
		void releaseButton(MouseButtons b);
		/** @} */


		/** @{ */
		/** @brief Set the cursor position of the mouse. */
		void setCursorPos(const Point2D& p) noexcept {
			m_cursorPos = p;
		}

		/** @brief Cursor position of the mouse. */
		const Point2D& cursorPos() const noexcept {
			return m_cursorPos;
		}
		/** @} */

	private:
		std::vector<MouseButtons> m_pressedButtons;
		Point2D m_cursorPos = { };
	};

}

#endif
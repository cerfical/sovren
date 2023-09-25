#ifndef RENI_MOUSE_STATE_HEADER
#define RENI_MOUSE_STATE_HEADER

#include "Utils.hpp"
#include "MouseButtons.hpp"

#include <vector>

namespace RENI {
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
		MouseState(const MouseState&) = default;
		MouseState& operator=(const MouseState&) = default;
		/** @} */


		/** @{ */
		/** @brief Get the pressed/released state of a mouse button. */
		bool operator[](MouseButtons button) const noexcept {
			return IsButtonPressed(button);
		}
		/** @} */


		/** @{ */
		/** @brief Check if a button is pressed. */
		bool IsButtonPressed(MouseButtons b) const noexcept;

		/** @brief Check if a button is released. */
		bool IsButtonReleased(MouseButtons b) const noexcept {
			return !IsButtonPressed(b);
		}

		/** @brief Check if any button is pressed. */
		bool AnyButtonPressed() const noexcept {
			return !m_pressedButtons.empty();
		}
		/** @} */


		/** @{ */
		/** @brief Toggle a button to the pressed state. */
		void PressButton(MouseButtons b);

		/** @brief Toggle a button to the released state. */
		void ReleaseButton(MouseButtons b);
		/** @} */


		/** @{ */
		/** @brief Set the cursor position of the mouse. */
		void SetCursorPos(const Point2D& p) noexcept {
			m_cursorPos = p;
		}

		/** @brief Get cursor position of the mouse. */
		const Point2D& GetCursorPos() const noexcept {
			return m_cursorPos;
		}
		/** @} */

	private:
		std::vector<MouseButtons> m_pressedButtons;
		Point2D m_cursorPos = { };
	};
}

#endif
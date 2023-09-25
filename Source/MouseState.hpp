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
		bool IsPressed(MouseButtons b) const noexcept;
		bool IsReleased(MouseButtons b) const noexcept {
			return !IsPressed(b);
		}

		bool AnyPressed() const noexcept {
			return !m_pressedButtons.empty();
		}
		/** @} */

		/** @{ */
		void Press(MouseButtons b);
		void Release(MouseButtons b);
		/** @} */

		/** @{ */
		void SetCursorPos(const Point2D& p) noexcept {
			m_cursorPos = p;
		}
		const Point2D& GetCursorPos() const noexcept {
			return m_cursorPos;
		}
		/** @} */

	private:
		std::vector<MouseButtons> m_pressedButtons;
		Point2D m_cursorPos;
	};
}

#endif
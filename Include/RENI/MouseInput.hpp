#ifndef RENI_MOUSE_INPUT_HEADER
#define RENI_MOUSE_INPUT_HEADER

#include "Utils.hpp"
#include <unordered_set>

namespace RENI {
	/**
	 * @brief Identifies available buttons on the mouse.
	 */
	enum class MouseButtons {
		Left,
		Middle,
		Right
	};

	/**
	 * @brief Mouse input data.
	 */
	class MouseInput {
	public:
		/** @{ */
		/** @brief Construct a new MouseInput object. */
		MouseInput() = default;
		/** @brief Destroy the MouseInput object. */
		~MouseInput() = default;
		/** @} */

		/** @{ */
		/** @brief Construct a new MouseInput object by moving from another MouseInput object. */
		MouseInput(MouseInput&&) = default;
		/** @brief Move another MouseInput object into the MouseInput. */
		MouseInput& operator=(MouseInput&&) = default;
		/** @} */

		/** @{ */
		MouseInput(const MouseInput&) = delete;
		MouseInput& operator=(const MouseInput&) = delete;
		/** @} */

		/** @{ */
		/** @brief Check if the button is in pressed state. */
		bool IsButtonPressed(MouseButtons button) const noexcept {
			return pressedButtons.contains(button);
		}
		/** @brief Check if the button is in released state. */
		bool IsButtonReleased(MouseButtons button) const noexcept {
			return !IsButtonPressed(button);
		}
		/** @brief Check if any button is in pressed state. */
		bool IsAnyButtonPressed() const noexcept {
			return !pressedButtons.empty();
		}

		/** @brief Toggle button to pressed state. */
		void PressButton(MouseButtons button) {
			pressedButtons.insert(button);
		}
		/** @brief Toggle button to released state. */
		void ReleaseButton(MouseButtons button) {
			pressedButtons.erase(button);
		}
		/** @} */

		/** @{ */
		/** @brief Update cursor position of the mouse. */
		void SetCursorPos(Point2D cursorPos) noexcept {
			this->cursorPos = cursorPos;
		}
		/** @brief Get cursor position of the mouse. */
		Point2D GetCursorPos() const noexcept {
			return cursorPos;
		}
		/** @} */

	private:
		std::unordered_set<MouseButtons> pressedButtons;
		Point2D cursorPos;
	};
}

#endif
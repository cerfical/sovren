#ifndef RENI_MOUSE_STATE_HEADER
#define RENI_MOUSE_STATE_HEADER

#include "Utils.hpp"

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
	 * @brief Interface for monitoring mouse events.
	 */
	class MouseObserver {
	public:
		/** @{ */
		/** @brief Called when a mouse button has been pressed. */
		virtual void OnMouseButtonPress(MouseButtons pressedButton) { }

		/** @brief Called when a mouse button has been released. */
		virtual void OnMouseButtonRelease(MouseButtons releasedButton) { }

		/** @brief Called when the mouse has been moved. */
		virtual void OnMouseMove(const Displace2D& mouseDisplace) { }
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new MouseObserver. */
		MouseObserver() = default;

		/** @brief Destroy the MouseObserver. */
		~MouseObserver() = default;
		/** @} */
	};

	/**
	 * @brief Accumulates information about the current state of the mouse.
	 */
	class MouseState {
	public:
		/** @{ */
		/** @brief Construct a new MouseState object. */
		MouseState() = default;

		/** @brief Destroy the MouseState object. */
		~MouseState() = default;
		/** @} */


		/** @{ */
		MouseState(MouseState&&) = delete;
		MouseState& operator=(MouseState&&) = delete;
		/** @} */


		/** @{ */
		MouseState(const MouseState&) = delete;
		MouseState& operator=(const MouseState&) = delete;
		/** @} */


		/** @{ */
		/** @brief Check if the button is in pressed state. */
		bool IsButtonPressed(MouseButtons button) const noexcept {
			return std::ranges::find(pressedButtons, button) != pressedButtons.cend();
		}

		/** @brief Check if the button is in released state. */
		bool IsButtonReleased(MouseButtons button) const noexcept {
			return !IsButtonPressed(button);
		}

		/** @brief Check if any button is pressed. */
		bool IsAnyButtonPressed() const noexcept {
			return !pressedButtons.empty();
		}


		/** @brief Toggle button to pressed state. */
		void PressButton(MouseButtons button) {
			if(IsButtonReleased(button)) {
				pressedButtons.push_back(button);
				observers.TriggerEvent(&MouseObserver::OnMouseButtonPress, button);
			}
		}

		/** @brief Toggle button to released state. */
		void ReleaseButton(MouseButtons button) {
			if(IsButtonPressed(button)) {
				std::erase(pressedButtons, button);
				observers.TriggerEvent(&MouseObserver::OnMouseButtonRelease, button);
			}
		}
		/** @} */

		/** @{ */
		/** @brief Update cursor position of the mouse. */
		void SetCursorPos(const Point2D& cursorPos) {
			const auto oldPos = std::exchange(this->cursorPos, cursorPos);
			if(oldPos != cursorPos) {
				const auto mouseDisplace = Displace2D(cursorPos.x - oldPos.x, cursorPos.y - oldPos.y);
				observers.TriggerEvent(&MouseObserver::OnMouseMove, mouseDisplace);
			}
		}

		/** @brief Get the current cursor position of the mouse. */
		const Point2D& GetCursorPos() const noexcept {
			return cursorPos;
		}
		/** @} */


		/** @{ */
		/** @brief Register a MouseObserver to be notified of changes to this MouseState object. */
		void AddObserver(MouseObserver& observer) const {
			observers.Add(observer);
		}

		/** @brief Unregister a MouseObserver from being notified of changes. */
		void RemoveObserver(MouseObserver& observer) const noexcept {
			observers.Remove(observer);
		}
		/** @} */

	private:
		mutable ObserverList<MouseObserver> observers;
		std::vector<MouseButtons> pressedButtons;

		Point2D cursorPos;
	};
}

#endif
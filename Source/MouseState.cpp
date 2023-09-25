#include "MouseState.hpp"
#include <algorithm>

namespace RENI {
	bool MouseState::IsButtonPressed(MouseButtons b) const noexcept {
		return std::ranges::find(m_pressedButtons, b) != m_pressedButtons.cend();
	}

	void MouseState::PressButton(MouseButtons b) {
		if(IsButtonReleased(b)) {
			m_pressedButtons.push_back(b);
		}
	}

	void MouseState::ReleaseButton(MouseButtons b) {
		if(IsButtonPressed(b)) {
			std::erase(m_pressedButtons, b);
		}
	}
}
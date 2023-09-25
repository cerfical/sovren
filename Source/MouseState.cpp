#include "MouseState.hpp"
#include <algorithm>

namespace RENI {
	bool MouseState::IsPressed(MouseButtons b) const noexcept {
		return std::ranges::find(m_pressedButtons, b) != m_pressedButtons.cend();
	}

	void MouseState::Press(MouseButtons b) {
		if(IsReleased(b)) {
			m_pressedButtons.push_back(b);
		}
	}

	void MouseState::Release(MouseButtons b) {
		if(IsPressed(b)) {
			std::erase(m_pressedButtons, b);
		}
	}
}
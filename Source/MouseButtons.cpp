#include "MouseButtons.hpp"
#include <algorithm>

namespace RENI {
	std::ostream& operator<<(std::ostream& out, MouseButtons button) {
#define RENI_MOUSE_BUTTON(b) case MouseButtons::b: out << "<" #b ">"; break;
		
		switch(button) {
			RENI_MOUSE_BUTTON_LIST
		}

#undef RENI_MOUSE_BUTTON
		return out;
	}

	bool MouseState::isButtonPressed(MouseButtons b) const noexcept {
		return std::ranges::find(m_pressedButtons, b) != m_pressedButtons.cend();
	}

	void MouseState::pressButton(MouseButtons b) {
		if(isButtonReleased(b)) {
			m_pressedButtons.push_back(b);
		}
	}

	void MouseState::releaseButton(MouseButtons b) {
		if(isButtonPressed(b)) {
			std::erase(m_pressedButtons, b);
		}
	}
}
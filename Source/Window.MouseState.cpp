#include "Window.hpp"
#include <algorithm>

namespace ranges = std::ranges;

namespace RENI {
	std::ostream& operator<<(std::ostream& out, MouseButtons button) {
		switch(button) {
#define RENI_MOUSE_BUTTON(button) case MouseButtons::button: out << "<" #button ">"; break;
			RENI_MOUSE_BUTTON_LIST
#undef RENI_MOUSE_BUTTON
		}
		return out;
	}


	bool Window::MouseState::IsButtonPressed(MouseButtons button) const noexcept {
		return ranges::find(m_pressedButtons, button) != m_pressedButtons.cend();
	}

	void Window::MouseState::PressButton(Window& window, MouseButtons button) {
		if(IsButtonReleased(button)) {
			m_pressedButtons.push_back(button);
			m_activeButton = button;
			window.OnButtonPress();
			m_activeButton = { };
		}
	}

	void Window::MouseState::ReleaseButton(Window& window, MouseButtons button) {
		if(IsButtonPressed(button)) {
			std::erase(m_pressedButtons, button);
			m_activeButton = button;
			window.OnButtonRelease();
			m_activeButton = { };
		}
	}

	void Window::MouseState::UpdateCursorPos(Window& window, const Point2D& cursorPos) {
		if(m_cursorPos != cursorPos) {
			m_oldCursorPos = std::exchange(m_cursorPos, cursorPos);
			window.OnMouseMove();
		}
	}
}
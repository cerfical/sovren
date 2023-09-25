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

	std::ostream& operator<<(std::ostream& out, Keys key) {
		switch(key) {
#define RENI_KEY(key) case Keys::key: out << "<" #key ">"; break;
			RENI_KEY_LIST
#undef RENI_KEY
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


	bool Window::KeysState::IsKeyPressed(Keys key) const noexcept {
		return ranges::find(m_pressedKeys, key) != m_pressedKeys.cend();
	}

	void Window::KeysState::PressKey(Window& window, Keys key) {
		if(IsKeyReleased(key)) {
			m_pressedKeys.push_back(key);
			m_activeKey = key;
			window.OnKeyPress();
			m_activeKey = { };
		}
	}

	void Window::KeysState::ReleaseKey(Window& window, Keys key) {
		if(IsKeyPressed(key)) {
			std::erase(m_pressedKeys, key);
			m_activeKey = key;
			window.OnKeyRelease();
			m_activeKey = { };
		}
	}


	void Window::UpdateClientArea(const Extent2D& clientArea) {
		if(m_clientArea != clientArea) {
			m_clientArea = clientArea;
			GetCanvas()->Resize(clientArea);
			OnResize();
		}
	}

	void Window::UpdateVisibility(bool visible) {
		if(m_visible != visible) {
			m_visible = visible;
			if(visible) {
				OnShow();
			} else {
				OnHide();
			}
		}
	}
}
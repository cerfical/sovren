#include "Window.hpp"
#include <algorithm>

namespace ranges = std::ranges;

namespace RENI {
	std::ostream& operator<<(std::ostream& out, Keys key) {
		switch(key) {
#define RENI_KEY(key) case Keys::key: out << "<" #key ">"; break;
			RENI_KEY_LIST
#undef RENI_KEY
		}
		return out;
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
}
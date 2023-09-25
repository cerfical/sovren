#include "KeysState.hpp"
#include <algorithm>

namespace RENI {
	bool KeysState::IsKeyPressed(Keys k) const noexcept {
		return std::ranges::find(m_pressedKeys, k) != m_pressedKeys.cend();
	}

	void KeysState::PressKey(Keys k) {
		if(IsKeyReleased(k)) {
			m_pressedKeys.push_back(k);
		}
	}

	void KeysState::ReleaseKey(Keys k) {
		if(IsKeyPressed(k)) {
			std::erase(m_pressedKeys, k);
		}
	}
}
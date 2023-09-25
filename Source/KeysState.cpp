#include "KeysState.hpp"
#include <algorithm>

namespace RENI {
	bool KeysState::IsPressed(Keys k) const noexcept {
		return std::ranges::find(m_pressedKeys, k) != m_pressedKeys.cend();
	}

	void KeysState::Press(Keys k) {
		if(IsReleased(k)) {
			m_pressedKeys.push_back(k);
		}
	}

	void KeysState::Release(Keys k) {
		if(IsPressed(k)) {
			std::erase(m_pressedKeys, k);
		}
	}
}
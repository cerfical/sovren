#include "Keys.hpp"
#include <algorithm>

namespace RENI {
	std::ostream& operator<<(std::ostream& out, Keys key) {
#define RENI_KEY(k) case Keys::k: out << "<" #k ">"; break;
		
		switch(key) {
			RENI_KEY_LIST
		}

#undef RENI_KEY
		return out;
	}

	bool KeysState::isKeyPressed(Keys k) const noexcept {
		return std::ranges::find(m_pressedKeys, k) != m_pressedKeys.cend();
	}

	void KeysState::pressKey(Keys k) {
		if(isKeyReleased(k)) {
			m_pressedKeys.push_back(k);
		}
	}

	void KeysState::releaseKey(Keys k) {
		if(isKeyPressed(k)) {
			std::erase(m_pressedKeys, k);
		}
	}
}
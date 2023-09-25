#ifndef RENI_KEYS_STATE_HEADER
#define RENI_KEYS_STATE_HEADER

#include "Keys.hpp"
#include <vector>

namespace RENI {
	/**
	 * @brief Contains information about the state of the keys on the keyboard.
	 */
	class KeysState {
	public:
		/** @{ */
		bool IsPressed(Keys k) const noexcept;
		bool IsReleased(Keys k) const noexcept {
			return !IsPressed(k);
		}

		bool AnyPressed() const noexcept {
			return !m_pressedKeys.empty();
		}
		/** @} */

		/** @{ */
		void Press(Keys k);
		void Release(Keys k);
		/** @} */

	private:
		std::vector<Keys> m_pressedKeys;
	};
}

#endif
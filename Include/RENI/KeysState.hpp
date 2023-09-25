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
		KeysState() = default;
		~KeysState() = default;
		/** @} */


		/** @{ */
		KeysState(KeysState&&) = default;
		KeysState& operator=(KeysState&&) = default;
		/** @} */

		/** @{ */
		KeysState(const KeysState&) = default;
		KeysState& operator=(const KeysState&) = default;
		/** @} */


		/** @{ */
		/** @brief Get the pressed/released state of a key. */
		bool operator[](Keys k) const noexcept {
			return IsKeyPressed(k);
		}
		/** @} */


		/** @{ */
		/** @brief Check if a key is pressed. */
		bool IsKeyPressed(Keys k) const noexcept;

		/** @brief Check if a key is released. */
		bool IsKeyReleased(Keys k) const noexcept {
			return !IsKeyPressed(k);
		}

		/** @brief Check if any key is pressed. */
		bool AnyKeyPressed() const noexcept {
			return !m_pressedKeys.empty();
		}
		/** @} */


		/** @{ */
		/** @brief Toggle a key to the pressed state. */
		void PressKey(Keys k);

		/** @brief Toggle a key to the released state. */
		void ReleaseKey(Keys k);
		/** @} */

	private:
		std::vector<Keys> m_pressedKeys;
	};
}

#endif
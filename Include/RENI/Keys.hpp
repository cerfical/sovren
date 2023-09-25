#ifndef RENI_KEYS_HEADER
#define RENI_KEYS_HEADER

#define RENI_KEY_LIST \
	RENI_KEY(None) /**< @brief Empty default value. */ \
	RENI_KEY(LeftArrow) /**< @brief Left arrow key. */ \
	RENI_KEY(RightArrow) /**< @brief Right arrow key. */ \
	RENI_KEY(UpArrow) /**< @brief Up arrow key. */ \
	RENI_KEY(DownArrow) /**< @brief Down arrow key. */

#include <ostream>
#include <vector>

namespace RENI {

#define RENI_KEY(b) b,
	
	/**
	 * @brief Identifies available keys on the keyboard.
	 */
	enum class Keys {
		RENI_KEY_LIST
	};

#undef RENI_KEY

	/** @brief Print a Keys enumerator to a @c std::ostream. */
	std::ostream& operator<<(std::ostream& out, Keys key);



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
		KeysState(const KeysState&) = delete;
		KeysState& operator=(const KeysState&) = delete;
		/** @} */


		/** @{ */
		/** @brief Get the pressed/released state of a key. */
		bool operator[](Keys k) const noexcept {
			return isKeyPressed(k);
		}
		/** @} */


		/** @{ */
		/** @brief Check if a key is pressed. */
		bool isKeyPressed(Keys k) const noexcept;

		/** @brief Check if a key is released. */
		bool isKeyReleased(Keys k) const noexcept {
			return !isKeyPressed(k);
		}

		/** @brief Check if any key is pressed. */
		bool anyKeyPressed() const noexcept {
			return !m_pressedKeys.empty();
		}
		/** @} */


		/** @{ */
		/** @brief Toggle a key to the pressed state. */
		void pressKey(Keys k);

		/** @brief Toggle a key to the released state. */
		void releaseKey(Keys k);
		/** @} */

	private:
		std::vector<Keys> m_pressedKeys;
	};
}

#endif
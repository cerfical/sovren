#ifndef RENI_KEYS_STATE_HEADER
#define RENI_KEYS_STATE_HEADER

#include "Utils.hpp"
#include <ostream>

#define RENI_KEY_LIST \
	RENI_KEY(LeftArrow, /**< @brief Left arrow key. */) \
	RENI_KEY(RightArrow, /**< @brief Right arrow key. */) \
	RENI_KEY(UpArrow, /**< @brief Up arrow key. */) \
	RENI_KEY(DownArrow, /**< @brief Down arrow key. */)

namespace RENI {
#define RENI_KEY(key, doc) key, doc
	/**
	 * @brief Identifies available keys on the keyboard.
	 */
	enum class Keys {
		RENI_KEY_LIST
	};
#undef RENI_KEY

	/** @brief Print a Keys enumerator to a @c std::ostream. */
	inline std::ostream& operator<<(std::ostream& out, Keys key) {
		switch(key) {
#define RENI_KEY(key, doc) case Keys::key: out << "<" #key ">"; break;
			RENI_KEY_LIST
#undef RENI_KEY
		}
		return out;
	}

	/**
	 * @brief Interface for monitoring keyboard events.
	 */
	class KeysObserver {
	public:
		/** @{ */
		/** @brief Called when a key has been pressed. */
		virtual void OnKeyPress(Keys pressedKey) { }

		/** @brief Called when a key has been released. */
		virtual void OnKeyRelease(Keys releasedKey) { }
		/** @} */

	protected:
		/** @{ */
		/** @brief Construct a new KeysObserver. */
		KeysObserver() = default;

		/** @brief Destroy the KeysObserver. */
		~KeysObserver() = default;
		/** @} */
	};

	/**
	 * @brief Accumulates information about the current state of the keys on the keyboard.
	 */
	class KeysState {
	public:
		/** @{ */
		/** @brief Construct a new KeysState object. */
		KeysState() = default;

		/** @brief Destroy the KeysState object. */
		~KeysState() = default;
		/** @} */


		/** @{ */
		KeysState(KeysState&&) = delete;
		KeysState& operator=(KeysState&&) = delete;
		/** @} */


		/** @{ */
		KeysState(const KeysState&) = delete;
		KeysState& operator=(const KeysState&) = delete;
		/** @} */


		/** @{ */
		/** @brief Check if the key is in pressed state. */
		bool IsKeyPressed(Keys key) const noexcept {
			return std::ranges::find(pressedKeys, key) != pressedKeys.cend();
		}

		/** @brief Check if the key is in released state. */
		bool IsKeyReleased(Keys key) const noexcept {
			return !IsKeyPressed(key);
		}

		/** @brief Check if any key is pressed. */
		bool IsAnyKeyPressed() const noexcept {
			return !pressedKeys.empty();
		}


		/** @brief Toggle a key to pressed state. */
		void PressKey(Keys key) {
			if(IsKeyReleased(key)) {
				pressedKeys.push_back(key);
				observers.TriggerEvent(&KeysObserver::OnKeyPress, key);
			}
		}

		/** @brief Toggle a key to released state. */
		void ReleaseKey(Keys key) {
			if(IsKeyPressed(key)) {
				std::erase(pressedKeys, key);
				observers.TriggerEvent(&KeysObserver::OnKeyRelease, key);
			}
		}
		/** @} */


		/** @{ */
		/** @brief Register a KeysObserver to be notified of changes to this KeysState object. */
		void AddObserver(KeysObserver& observer) const {
			observers.Add(observer);
		}

		/** @brief Unregister a KeysObserver from being notified of changes. */
		void RemoveObserver(KeysObserver& observer) const noexcept {
			observers.Remove(observer);
		}
		/** @} */

	private:
		mutable ObserverList<KeysObserver> observers;
		std::vector<Keys> pressedKeys;
	};
}

#endif
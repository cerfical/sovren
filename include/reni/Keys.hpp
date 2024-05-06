#pragma once

#define RENI_KEY_LIST \
	RENI_KEY(None) /**< @brief Empty default value. */ \
	RENI_KEY(LeftArrow) /**< @brief Left arrow key. */ \
	RENI_KEY(RightArrow) /**< @brief Right arrow key. */ \
	RENI_KEY(UpArrow) /**< @brief Up arrow key. */ \
	RENI_KEY(DownArrow) /**< @brief Down arrow key. */

namespace reni {

#define RENI_KEY(k) k,
	
	/**
	 * @brief Identifies available keys on the keyboard.
	*/
	enum class Keys {
		RENI_KEY_LIST
	};

#undef RENI_KEY

}
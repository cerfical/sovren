#ifndef RENI_KEYS_HEADER
#define RENI_KEYS_HEADER

#define RENI_KEY_LIST \
	RENI_KEY(None) /**< @brief Empty default value. */ \
	RENI_KEY(LeftArrow) /**< @brief Left arrow key. */ \
	RENI_KEY(RightArrow) /**< @brief Right arrow key. */ \
	RENI_KEY(UpArrow) /**< @brief Up arrow key. */ \
	RENI_KEY(DownArrow) /**< @brief Down arrow key. */

#include <ostream>

namespace RENI {

#define RENI_KEY(b) b,
	
	/**
	 * @brief Identifies available keys on the keyboard.
	*/
	enum class Keys {
		RENI_KEY_LIST
	};

#undef RENI_KEY


	std::ostream& operator<<(std::ostream& out, Keys k);

}

#endif
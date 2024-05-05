#ifndef RENI_MOUSE_BUTTONS_HEADER
#define RENI_MOUSE_BUTTONS_HEADER

#define RENI_MOUSE_BUTTON_LIST \
	RENI_MOUSE_BUTTON(None) /**< @brief Empty default value. */ \
	RENI_MOUSE_BUTTON(Left) /**< @brief Left mouse button. */ \
	RENI_MOUSE_BUTTON(Middle) /**< @brief Middle mouse button. */ \
	RENI_MOUSE_BUTTON(Right) /**< @brief Right mouse button. */

namespace reni {

#define RENI_MOUSE_BUTTON(b) b,
	
	/**
	 * @brief Identifies available buttons on the mouse.
	*/
	enum class MouseButtons {
		RENI_MOUSE_BUTTON_LIST
	};

#undef RENI_MOUSE_BUTTON

}

#endif
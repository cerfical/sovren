#ifndef RENI_PAL_WINDOW_CALLBACKS_HEADER
#define RENI_PAL_WINDOW_CALLBACKS_HEADER

#include "../Keys.hpp"
#include "../MouseButtons.hpp"
#include "../util/types.hpp"

namespace reni::pal {

	class WindowCallbacks {
	public:

		virtual void onWindowClose() = 0;

		virtual void onWindowResize(Size2 newSize) = 0;


		virtual void onKeyStateChange(Keys key, bool pressed) = 0;


		virtual void onMouseButtonStateChange(MouseButtons button, bool pressed) = 0;

		virtual void onMouseMove(Point2 newPos) = 0;

	protected:
		~WindowCallbacks() = default;
	};

}

#endif
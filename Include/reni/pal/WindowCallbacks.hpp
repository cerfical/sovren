#ifndef RENI_PAL_WINDOW_CALLBACKS_HEADER
#define RENI_PAL_WINDOW_CALLBACKS_HEADER

#include "../Keys.hpp"
#include "../MouseButtons.hpp"
#include "../Point2D.hpp"
#include "../Size2D.hpp"

namespace reni::pal {

	class WindowCallbacks {
	public:

		virtual void onWindowClose() = 0;

		virtual void onWindowResize(Size2D newSize) = 0;


		virtual void onKeyStateChange(Keys key, bool pressed) = 0;


		virtual void onMouseButtonStateChange(MouseButtons button, bool pressed) = 0;

		virtual void onMouseMove(Point2D newPos) = 0;

	protected:
		~WindowCallbacks() = default;
	};

}

#endif
#ifndef RENI_PAL_WINDOW_HEADER
#define RENI_PAL_WINDOW_HEADER

#include "../util/types.hpp"

#include <string_view>
#include <string>

namespace reni::pal {

	class WindowCallbacks;


	class Window : private NonCopyable, private NonMovable {
	public:

		virtual ~Window() = default;


		virtual void installCallbacks(WindowCallbacks* callbacks) = 0;


		virtual void setTitle(std::string_view newTitle) = 0;

		virtual void setClientSize(Size2D newSize) = 0;
		
		virtual void setVisible(bool visible) = 0;


		virtual std::string getTitle() const = 0;
		
		virtual Size2D getClientSize() const = 0;
		
		virtual bool isVisible() const = 0;


		virtual Point2D getMousePos() const = 0;

		virtual void* nativeHandle() const = 0;

	};

}

#endif
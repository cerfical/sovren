#include "MouseButtons.hpp"

namespace reni {
	std::ostream& operator<<(std::ostream& out, MouseButtons b) {
#define RENI_MOUSE_BUTTON(b) case MouseButtons::b: out << "<" #b ">"; break;
		
		switch(b) {
			RENI_MOUSE_BUTTON_LIST
		}

#undef RENI_MOUSE_BUTTON
		return out;
	}
}
#include "MouseButtons.hpp"

namespace RENI {
	std::ostream& operator<<(std::ostream& out, MouseButtons button) {
#define RENI_MOUSE_BUTTON(b) case MouseButtons::b: out << "<" #b ">"; break;
		
		switch(button) {
			RENI_MOUSE_BUTTON_LIST
		}

#undef RENI_MOUSE_BUTTON
		return out;
	}
}
#include "print.hpp"

#include "util/types.hpp"

#include "MouseButtons.hpp"
#include "Keys.hpp"

#include <iomanip>

namespace reni {
	std::ostream& operator<<(std::ostream& out, Keys k) {
#define RENI_KEY(k) case Keys::k: out << "<" #k ">"; break;
		switch(k) {
			RENI_KEY_LIST
		}
#undef RENI_KEY
		return out;
	}


	std::ostream& operator<<(std::ostream& out, MouseButtons b) {
#define RENI_MOUSE_BUTTON(b) case MouseButtons::b: out << "<" #b ">"; break;

		switch(b) {
			RENI_MOUSE_BUTTON_LIST
		}

#undef RENI_MOUSE_BUTTON
		return out;
	}


	std::ostream& operator<<(std::ostream& out, Size2 s) {
		return out << '(' << s.width << ", " << s.height << ')';
	}


	std::ostream& operator<<(std::ostream& out, Point2 p) {
		return out << '(' << p.x << ", " << p.y << ')';
	}


	std::ostream& operator<<(std::ostream& out, Color c) {
		const auto fmtFlags = out.flags();
		out << '#' << std::hex << std::setfill('0')
			<< std::setw(2) << static_cast<int>(c.red)
			<< std::setw(2) << static_cast<int>(c.green)
			<< std::setw(2) << static_cast<int>(c.blue)
			<< std::setw(2) << static_cast<int>(c.alpha);
		out.flags(fmtFlags);
		return out;
	}
}
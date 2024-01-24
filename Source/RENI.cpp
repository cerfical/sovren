#include "Size2D.hpp"
#include "Point2D.hpp"
#include "Color.hpp"

#include <iomanip>

namespace RENI {
	std::ostream& operator<<(std::ostream& out, Size2D s) {
		return out << '(' << s.width << ", " << s.height << ')';
	}

	std::ostream& operator<<(std::ostream& out, Point2D p) {
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
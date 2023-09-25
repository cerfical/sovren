#include "Utils.hpp"
#include <iomanip>

namespace RENI {
	std::ostream& operator<<(std::ostream& out, const Size2D& size) {
		return out << '(' << size.width() << ", " << size.height() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Point2D& point) {
		return out << '(' << point.x() << ", " << point.y() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Offset2D& offset) {
		return out << '(' << offset.deltaX() << ", " << offset.deltaY() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Rect2D& rect) {
		return out << '(' << rect.topLeft() << ", " << rect.bottomRight() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Line2D& line) {
		return out << '(' << line.start() << ", " << line.end() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Color& color) {
		const auto fmtFlags = out.flags();
		out << '#' << std::hex << std::setfill('0')
			<< std::setw(2) << static_cast<int>(color.red())
			<< std::setw(2) << static_cast<int>(color.green())
			<< std::setw(2) << static_cast<int>(color.blue())
			<< std::setw(2) << static_cast<int>(color.alpha());
		out.flags(fmtFlags);
		return out;
	}
}
#include "Utils.hpp"
#include <iomanip>

namespace RENI {
	std::ostream& operator<<(std::ostream& out, const Size2D& size) {
		return out << '(' << size.GetWidth() << ", " << size.GetHeight() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Point2D& point) {
		return out << '(' << point.GetX() << ", " << point.GetY() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Offset2D& offset) {
		return out << '(' << offset.GetDeltaX() << ", " << offset.GetDeltaY() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Rect2D& rect) {
		return out << '(' << rect.GetTopLeft() << ", " << rect.GetBottomRight() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Line2D& line) {
		return out << '(' << line.GetStart() << ", " << line.GetEnd() << ')';
	}

	std::ostream& operator<<(std::ostream& out, const Color& color) {
		const auto fmtFlags = out.flags();
		out << '#' << std::hex << std::setfill('0')
			<< std::setw(2) << static_cast<int>(color.GetRed())
			<< std::setw(2) << static_cast<int>(color.GetGreen())
			<< std::setw(2) << static_cast<int>(color.GetBlue())
			<< std::setw(2) << static_cast<int>(color.GetAlpha());
		out.flags(fmtFlags);
		return out;
	}
}
#ifndef RENI_PRINT_HEADER
#define RENI_PRINT_HEADER

#include <iosfwd>

namespace reni {

	enum class Keys;
	enum class MouseButtons;

	struct Size2D;
	struct Point2D;
	struct Color;


	std::ostream& operator<<(std::ostream& out, Keys k);

	std::ostream& operator<<(std::ostream& out, MouseButtons b);


	std::ostream& operator<<(std::ostream& out, Size2D s);

	std::ostream& operator<<(std::ostream& out, Point2D p);

	std::ostream& operator<<(std::ostream& out, Color c);

}

#endif
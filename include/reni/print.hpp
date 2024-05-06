#pragma once

#include <iosfwd>

namespace reni {

	enum class Keys;
	enum class MouseButtons;

	struct Size2;
	struct Point2;
	struct Color;


	std::ostream& operator<<(std::ostream& out, Keys k);

	std::ostream& operator<<(std::ostream& out, MouseButtons b);


	std::ostream& operator<<(std::ostream& out, Size2 s);

	std::ostream& operator<<(std::ostream& out, Point2 p);

	std::ostream& operator<<(std::ostream& out, Color c);

}
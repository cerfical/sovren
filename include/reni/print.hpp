#pragma once

#include <iosfwd>

namespace reni {

	enum class Keys;
	enum class MouseButtons;

	struct Size2;
	struct Point2;
	struct Color;

	struct Vec2;
	struct Vec3;
	struct Vec4;
	struct Mat4x4;


	std::ostream& operator<<(std::ostream& out, Keys k);

	std::ostream& operator<<(std::ostream& out, MouseButtons b);


	std::ostream& operator<<(std::ostream& out, Size2 s);

	std::ostream& operator<<(std::ostream& out, Point2 p);

	std::ostream& operator<<(std::ostream& out, Color c);


	std::ostream& operator<<(std::ostream& out, Vec2 v);
	
	std::ostream& operator<<(std::ostream& out, Vec3 v);
	
	std::ostream& operator<<(std::ostream& out, Vec4 v);

	std::ostream& operator<<(std::ostream& out, const Mat4x4& m);

}
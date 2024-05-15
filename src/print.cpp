#include "print.hpp"

#include "core/Keys.hpp"
#include "core/MouseButtons.hpp"

#include "util/types.hpp"
#include "math/types.hpp"

#include <format>
#include <ostream>

namespace reni {
	std::ostream& operator<<(std::ostream& out, Keys k) {

#define RENI_KEY(k) case Keys::k: return out << std::format("key({})", #k);

		switch(k) {
			RENI_KEY_LIST
		}

#undef RENI_KEY
		
		return out;
	}


	std::ostream& operator<<(std::ostream& out, MouseButtons b) {

#define RENI_MOUSE_BUTTON(b) case MouseButtons::b: return out << std::format("mouse({})", #b);

		switch(b) {
			RENI_MOUSE_BUTTON_LIST
		}

#undef RENI_MOUSE_BUTTON

		return out;
	}


	std::ostream& operator<<(std::ostream& out, Size2 s) {
		return out << std::format("size2({}, {})", s.width, s.height);
	}


	std::ostream& operator<<(std::ostream& out, Point2 p) {
		return out << std::format("point2({}, {})", p.x, p.y);
	}


	std::ostream& operator<<(std::ostream& out, Color c) {
		return out << std::format("rgba({}, {}, {}, {})", c.r, c.g, c.b, c.a);
	}


	std::ostream& operator<<(std::ostream& out, Vec2 v) {
		return out << std::format("vec2({}, {})", v.x, v.y);
	}
	

	std::ostream& operator<<(std::ostream& out, Vec3 v) {
		return out << std::format("vec3({}, {}, {})", v.x, v.y, v.z);
	}
	

	std::ostream& operator<<(std::ostream& out, Vec4 v) {
		return out << std::format("vec4({}, {}, {}, {})", v.x, v.y, v.z, v.w);
	}


	std::ostream& operator<<(std::ostream& out, const Mat4x4& m) {
		out << "mat4x4(";
		for(bool first = true; const auto& r : m) {
			if(!first) {
				out << ", ";
			} else {
				first = false;
			}
			out << r;
		}
		out << ')';
		return out;
	}


	std::ostream& operator<<(std::ostream& out, const Mat3x3& m) {
		out << "mat3x3(";
		for(bool first = true; const auto& r : m) {
			if(!first) {
				out << ", ";
			} else {
				first = false;
			}
			out << r;
		}
		out << ')';
		return out;
	}


	std::ostream& operator<<(std::ostream& out, const Mat2x2& m) {
		out << "mat2x2(";
		for(bool first = true; const auto& r : m) {
			if(!first) {
				out << ", ";
			} else {
				first = false;
			}
			out << r;
		}
		out << ')';
		return out;
	}
}
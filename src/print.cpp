#include "print.hpp"

#include "core/Key.hpp"
#include "core/MouseButton.hpp"

#include "math/Mat2x2.hpp"
#include "math/Mat3x3.hpp"
#include "math/Mat4x4.hpp"
#include "math/Vec2.hpp"
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"

#include "util/Color.hpp"
#include "util/Displace2.hpp"
#include "util/Displace3.hpp"
#include "util/Point2.hpp"
#include "util/Point3.hpp"
#include "util/Size2.hpp"
#include "util/Size3.hpp"

#include <format>
#include <ostream>

namespace sovren {
    std::ostream& operator<<(std::ostream& out, Key k) {
        switch(k) {
            case Key::None:       out << "None"; break;
            case Key::LeftArrow:  out << "LeftArrow"; break;
            case Key::RightArrow: out << "RightArrow"; break;
            case Key::UpArrow:    out << "UpArrow"; break;
            case Key::DownArrow:  out << "DownArrow"; break;
        }
        return out;
    }


    std::ostream& operator<<(std::ostream& out, MouseButton b) {
        switch(b) {
            case MouseButton::None:   out << "None"; break;
            case MouseButton::Left:   out << "Left"; break;
            case MouseButton::Middle: out << "Middle"; break;
            case MouseButton::Right:  out << "Right"; break;
        }
        return out;
    }


    std::ostream& operator<<(std::ostream& out, Size2 s) {
        return out << std::format("size2({}, {})", s.width, s.height);
    }


    std::ostream& operator<<(std::ostream& out, Size3 s) {
        return out << std::format("size3({}, {}, {})", s.width, s.height, s.depth);
    }


    std::ostream& operator<<(std::ostream& out, Point2 p) {
        return out << std::format("point2({}, {})", p.x, p.y);
    }


    std::ostream& operator<<(std::ostream& out, Point3 p) {
        return out << std::format("point3({}, {}, {})", p.x, p.y, p.z);
    }


    std::ostream& operator<<(std::ostream& out, Displace2 d) {
        return out << std::format("displace2({}, {})", d.dx, d.dy);
    }


    std::ostream& operator<<(std::ostream& out, Displace3 d) {
        return out << std::format("displace3({}, {}, {})", d.dx, d.dy, d.dz);
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

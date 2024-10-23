#include "print.hpp"

#include "math/Mat2x2.hpp"
#include "math/Mat3x3.hpp"
#include "math/Mat4x4.hpp"
#include "math/Vec2.hpp"
#include "math/Vec3.hpp"
#include "math/Vec4.hpp"

#include <format>
#include <ostream>

namespace sovren {
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

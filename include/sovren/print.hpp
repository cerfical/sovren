#pragma once

#include <iosfwd>

namespace sovren {

    struct Vec4;
    struct Vec3;
    struct Vec2;

    struct Mat4x4;
    struct Mat3x3;
    struct Mat2x2;


    std::ostream& operator<<(std::ostream& out, Vec2 v);

    std::ostream& operator<<(std::ostream& out, Vec3 v);

    std::ostream& operator<<(std::ostream& out, Vec4 v);


    std::ostream& operator<<(std::ostream& out, const Mat4x4& m);

    std::ostream& operator<<(std::ostream& out, const Mat3x3& m);

    std::ostream& operator<<(std::ostream& out, const Mat2x2& m);

}

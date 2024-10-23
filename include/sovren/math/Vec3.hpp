#pragma once

#include "Vec2.hpp"
#include "VecBase.hpp"

#include <iterator>

namespace sovren {

    struct Mat3x3;


    struct Vec3 : public math::VecBase<Vec3, Mat3x3, 3> {

        friend float* begin(Vec3& v) noexcept {
            return &v.x;
        }

        friend float* end(Vec3& v) noexcept {
            return std::next(&v.z);
        }


        Vec3() noexcept = default;


        Vec3(Vec2 xy, float z) noexcept
            : x(xy.x), y(xy.y), z(z) {}


        Vec3(float x, float y, float z) noexcept
            : Vec3(Vec2(x, y), z) {}


        Vec3 cross(Vec3 rhs) const noexcept {
            return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x };
        }


        Vec2 xy() const noexcept {
            return { x, y };
        }

        void setXy(Vec2 xy) noexcept {
            x = xy.x;
            y = xy.y;
        }


        float x = {};
        float y = {};
        float z = {};
    };

}

#pragma once

#include "VecBase.hpp"

#include <iterator>

namespace reni {

    struct Mat3x3;


    struct Vec3 : public math::VecBase<Vec3, Mat3x3, 3> {

        friend float* begin(Vec3& v) noexcept { return &v.x; }

        friend float* end(Vec3& v) noexcept { return std::next(&v.z); }


        Vec3() noexcept = default;

        Vec3(float x, float y, float z) noexcept
            : x(x), y(y), z(z) {}


        Vec3 cross(Vec3 rhs) const noexcept {
            return { y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x };
        }


        float x = {};
        float y = {};
        float z = {};
    };

}

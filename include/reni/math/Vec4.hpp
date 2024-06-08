#pragma once

#include "Vec3.hpp"
#include "VecBase.hpp"

#include <iterator>

namespace reni {

    struct Mat4x4;


    struct Vec4 : public math::VecBase<Vec4, Mat4x4, 4> {

        friend float* begin(Vec4& v) noexcept {
            return &v.x;
        }

        friend float* end(Vec4& v) noexcept {
            return std::next(&v.w);
        }


        Vec4() noexcept = default;

        Vec4(Vec3 xyz, float w) noexcept
            : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}

        Vec4(float x, float y, float z, float w) noexcept
            : Vec4(Vec3(x, y, z), w) {}


        Vec3 xyz() const noexcept {
            return { x, y, z };
        }

        void setXyz(Vec3 xyz) noexcept {
            x = xyz.x;
            y = xyz.y;
            z = xyz.z;
        }


        float x = {};
        float y = {};
        float z = {};
        float w = {};
    };

}

#pragma once

#include "VecBase.hpp"

#include <iterator>

namespace reni {

    struct Mat4x4;


    struct Vec4 : public math::VecBase<Vec4, Mat4x4, 4> {

        friend float* begin(Vec4& v) noexcept { return &v.x; }

        friend float* end(Vec4& v) noexcept { return std::next(&v.w); }


        Vec4() noexcept = default;

        Vec4(float x, float y, float z, float w) noexcept
            : x(x), y(y), z(z), w(w) {}


        float x = {};
        float y = {};
        float z = {};
        float w = {};
    };

}

#pragma once

#include "VecBase.hpp"

#include <iterator>

namespace sovren {

    struct Mat2x2;


    struct Vec2 : public math::VecBase<Vec2, Mat2x2, 2> {

        friend float* begin(Vec2& v) noexcept {
            return &v.x;
        }

        friend float* end(Vec2& v) noexcept {
            return std::next(&v.y);
        }


        Vec2() noexcept = default;

        Vec2(float x, float y) noexcept
            : x(x), y(y) {}


        float x = {};
        float y = {};
    };

}

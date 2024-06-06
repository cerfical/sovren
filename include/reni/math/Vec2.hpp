#pragma once

#include "VecBase.hpp"
#include "vec_ops.hpp"

#include <iterator>

namespace reni {

    struct Mat2x2;


    struct Vec2 : public math::VecBase<Vec2, Mat2x2> {

        friend auto operator<=>(Vec2, Vec2) noexcept = default;

        static inline constexpr int Order = 2;


        Vec2() noexcept = default;

        Vec2(float x, float y) noexcept
            : x(x), y(y) {}


        float operator[](int i) const noexcept {
            return const_cast<Vec2&>(*this)[i];
        }

        float& operator[](int i) noexcept {
            return *std::next(&x, i);
        }


        float x = {};
        float y = {};
    };

}

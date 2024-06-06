#pragma once

#include "VecBase.hpp"
#include "vec_ops.hpp"

#include <iterator>

namespace reni {

    struct Mat4x4;


    struct Vec4 : public math::VecBase<Vec4, Mat4x4> {

        friend auto operator<=>(Vec4, Vec4) noexcept = default;

        static inline constexpr int Order = 4;


        Vec4() noexcept = default;

        Vec4(float x, float y, float z, float w) noexcept
            : x(x), y(y), z(z), w(w) {}


        float operator[](int i) const noexcept {
            return const_cast<Vec4&>(*this)[i];
        }

        float& operator[](int i) noexcept {
            return *std::next(&x, i);
        }


        float x = {};
        float y = {};
        float z = {};
        float w = {};
    };

}

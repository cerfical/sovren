#pragma once

#include "VecBase.hpp"
#include "vec_ops.hpp"

#include <iterator>

namespace reni {

    struct Mat3x3;


    struct Vec3 : public math::VecBase<Vec3, Mat3x3> {

        friend auto operator<=>(Vec3, Vec3) noexcept = default;

        static inline constexpr int Order = 3;


        Vec3() noexcept = default;

        Vec3(float x, float y, float z) noexcept
            : x(x), y(y), z(z) {}


        float operator[](int i) const noexcept {
            return const_cast<Vec3&>(*this)[i];
        }

        float& operator[](int i) noexcept {
            return *std::next(&x, i);
        }


        float x = {};
        float y = {};
        float z = {};
    };

}

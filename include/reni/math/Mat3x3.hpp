#pragma once

#include "MatBase.hpp"
#include "Vec3.hpp"

#include <array>
#include <iterator>

namespace reni {

    struct Mat3x3 : public math::MatBase<Mat3x3, Vec3, 3> {

        friend Vec3* begin(Mat3x3& m) noexcept { return m.rows.data(); }

        friend Vec3* end(Mat3x3& m) noexcept { return std::next(begin(m), Order); }


        static Mat3x3 translation(float dx, float dy) noexcept {
            return {
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                {   dx,   dy, 1.0f }
            };
        }


        Mat3x3() noexcept = default;

        Mat3x3(Vec3 r1, Vec3 r2, Vec3 r3) noexcept
            : rows{ r1, r2, r3 } {}


        std::array<Vec3, Order> rows;
    };

}

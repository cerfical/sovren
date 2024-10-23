#pragma once

#include "MatBase.hpp"
#include "Vec2.hpp"

#include <iterator>

namespace sovren {

    struct Mat2x2 : public math::MatBase<Mat2x2, Vec2, 2> {

        friend Vec2* begin(Mat2x2& m) noexcept {
            return &m.r1;
        }

        friend Vec2* end(Mat2x2& m) noexcept {
            return std::next(begin(m), Order);
        }


        Mat2x2() noexcept = default;

        Mat2x2(Vec2 r1, Vec2 r2) noexcept
            : r1(r1), r2(r2) {}


        Vec2 r1;
        Vec2 r2;
    };

}

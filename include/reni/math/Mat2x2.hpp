#pragma once

#include "MatBase.hpp"
#include "Vec2.hpp"

#include <array>
#include <iterator>

namespace reni {

    struct Mat2x2 : public math::MatBase<Mat2x2, Vec2, 2> {

        friend Vec2* begin(Mat2x2& m) noexcept { return m.rows.data(); }

        friend Vec2* end(Mat2x2& m) noexcept { return std::next(begin(m), Order); }


        Mat2x2() noexcept = default;

        Mat2x2(Vec2 r1, Vec2 r2) noexcept
            : rows{ r1, r2 } {}


        std::array<Vec2, Order> rows;
    };

}

#pragma once

#include "MatBase.hpp"
#include "Vec2.hpp"

#include <array>
#include <iterator>

namespace reni {

    struct Mat2x2 : public math::MatBase<Mat2x2, float, Vec2> {

        friend bool operator==(const Mat2x2&, const Mat2x2&) noexcept = default;


        friend Vec2* begin(Mat2x2& m) noexcept { return m.rows.data(); }

        friend Vec2* end(Mat2x2& m) noexcept { return std::next(begin(m), static_cast<int>(m.rows.size())); }


        Mat2x2() noexcept = default;

        Mat2x2(Vec2 r1, Vec2 r2) noexcept
            : rows{ r1, r2 } {}


        std::array<Vec2, 2> rows;
    };

}

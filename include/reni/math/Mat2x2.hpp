#pragma once

#include "MatBase.hpp"
#include "Vec2.hpp"
#include "mat_ops.hpp"

#include <array>

namespace reni {

    struct Mat2x2 : public math::MatBase<Mat2x2, float, Vec2> {

        friend bool operator==(const Mat2x2&, const Mat2x2&) noexcept = default;

        static inline constexpr int Order = 2;


        Mat2x2() noexcept = default;

        Mat2x2(Vec2 r1, Vec2 r2) noexcept
            : rows{ r1, r2 } {}


        const Vec2& operator[](int i) const noexcept {
            return const_cast<Mat2x2&>(*this)[i];
        }

        Vec2& operator[](int i) noexcept {
            return rows[i];
        }


        std::array<Vec2, Order> rows;
    };

}

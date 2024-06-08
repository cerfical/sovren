#pragma once

#include "MatBase.hpp"
#include "Vec2.hpp"
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


        Vec2 transformNormal(Vec2 norm) const noexcept {
            auto res = Vec2();
            // treat the 2x2 submatrix as the rotation/scale component of a transform
            for(int i = 0; i < Order - 1; i++) {
                for(int j = 0; j < Order - 1; j++) {
                    res[j] += norm[i] * (*this)[i][j];
                }
            }
            return res;
        }


        Vec2 transformCoord(Vec2 coord) const noexcept {
            auto res = transformNormal(coord);
            // add the translation component from the last row of the matrix
            for(int j = 0; j < Order - 1; j++) {
                res[j] += (*this)[Order - 1][j];
            }
            return res;
        }


        std::array<Vec3, Order> rows;
    };

}

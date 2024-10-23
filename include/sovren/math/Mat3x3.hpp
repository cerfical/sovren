#pragma once

#include "MatBase.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"

#include <iterator>

namespace sovren {

    struct Mat3x3 : public math::MatBase<Mat3x3, Vec3, 3> {

        friend Vec3* begin(Mat3x3& m) noexcept {
            return &m.r1;
        }

        friend Vec3* end(Mat3x3& m) noexcept {
            return std::next(begin(m), Order);
        }


        static Mat3x3 translation(float dx, float dy) noexcept {
            return {
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                {   dx,   dy, 1.0f }
            };
        }


        Mat3x3() noexcept = default;


        Mat3x3(Vec3 r1, Vec3 r2, Vec3 r3) noexcept
            : r1(r1), r2(r2), r3(r3) {}


        Vec2 transformNormal(Vec2 norm) const noexcept {
            // treat the 2x2 submatrix as the rotation/scale component of a transform
            return Vec2::splat(norm.x) * r1.xy() + Vec2::splat(norm.y) * r2.xy();
        }


        Vec2 transformCoord(Vec2 coord) const noexcept {
            // scale and rotate the coordinate point, then add in the translation component
            return transformNormal(coord) + r3.xy();
        }


        Vec3 r1;
        Vec3 r2;
        Vec3 r3;
    };

}

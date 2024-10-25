#pragma once

#include "Vec2.hpp"
#include "Vec3.hpp"
#include "mat_ops.hpp"

namespace sovren {

    struct Mat3x3 {

        [[nodiscard]]
        static consteval auto order() noexcept -> int {
            return 3;
        }

        [[nodiscard]]
        static auto identity() noexcept -> Mat3x3 {
            return makeIdentityMatrix<Mat3x3>();
        }


        [[nodiscard]]
        static auto translation(float dx, float dy) noexcept -> Mat3x3 {
            auto mat = identity();
            mat.r3.setXy(Vec2(dx, dy));
            return mat;
        }


        [[nodiscard]]
        auto operator[](int i) noexcept -> Vec3& {
            return *(&r1 + i);
        }

        [[nodiscard]]
        auto operator[](int i) const noexcept -> const Vec3& {
            return const_cast<Mat3x3&>(*this)[i];
        }


        [[nodiscard]]
        auto transformNormal(Vec2 norm) const noexcept -> Vec2 {
            // treat the 2x2 submatrix as the rotation/scale component of a transform
            return Vec2::splat(norm.x) * r1.xy() + Vec2::splat(norm.y) * r2.xy();
        }

        [[nodiscard]]
        auto transformCoord(Vec2 coord) const noexcept -> Vec2 {
            // scale and rotate the coordinate point, then add in the translation component
            return transformNormal(coord) + r3.xy();
        }


        Vec3 r1;
        Vec3 r2;
        Vec3 r3;
    };

}

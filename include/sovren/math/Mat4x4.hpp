#pragma once

#include "../Size2.hpp"

#include "Vec3.hpp"
#include "Vec4.hpp"
#include "mat_ops.hpp"

#include <cmath>

namespace sovren {

    struct Mat4x4 {

        [[nodiscard]]
        static consteval auto order() noexcept -> int {
            return 4;
        }

        [[nodiscard]]
        static auto identity() noexcept -> Mat4x4 {
            return makeIdentityMatrix<Mat4x4>();
        }


        [[nodiscard]]
        static auto translation(float dx, float dy, float dz) noexcept -> Mat4x4 {
            auto mat = identity();
            mat.r4.setXyz(Vec3(dx, dy, dz));
            return mat;
        }

        [[nodiscard]]
        static auto rotationX(float angle) noexcept -> Mat4x4 {
            const auto s = std::sin(angle);
            const auto c = std::cos(angle);

            return {
                { 1,  0, 0, 0 },
                { 0,  c, s, 0 },
                { 0, -s, c, 0 },
                { 0,  0, 0, 1 }
            };
        }

        [[nodiscard]]
        static auto rotationY(float angle) noexcept -> Mat4x4 {
            const auto s = std::sin(angle);
            const auto c = std::cos(angle);

            return {
                { c, 0, -s, 0 },
                { 0, 1,  0, 0 },
                { s, 0,  c, 0 },
                { 0, 0,  0, 1 }
            };
        }

        [[nodiscard]]
        static auto rotationZ(float angle) noexcept -> Mat4x4 {
            const auto s = std::sin(angle);
            const auto c = std::cos(angle);

            return {
                {  c, s, 0, 0 },
                { -s, c, 0, 0 },
                {  0, 0, 1, 0 },
                {  0, 0, 0, 1 }
            };
        }

        [[nodiscard]]
        static auto rotationAxis(Vec3 axis, float angle) noexcept -> Mat4x4 {
            const auto c = std::cos(angle);
            const auto s = std::sin(angle);

            const auto c1 = (1 - c);

            const auto ux = axis.x;
            const auto uy = axis.y;
            const auto uz = axis.z;

            return {
                {        c + (ux * ux * c1), (uy * ux * c1) + (uz * s), (uz * ux * c1) - (uy * s), 0 },
                { (ux * uy * c1) - (uz * s),        c + (uy * uy * c1), (uz * uy * c1) + (ux * s), 0 },
                { (ux * uz * c1) + (uy * s), (uy * uz * c1) - (ux * s),        c + (uz * uz * c1), 0 },
                {                         0,                         0,                         0, 1 }
            };
        }


        [[nodiscard]]
        static auto perspective(float aspectRatio, float fov, float nearPlane, float farPlane) noexcept -> Mat4x4 {
            const auto c1 = 1 / std::tan(fov / 2);
            const auto c2 = farPlane / (farPlane - nearPlane);

            return {
                { c1 / aspectRatio,  0,               0, 0 },
                {                0, c1,               0, 0 },
                {                0,  0,              c2, 1 },
                {                0,  0, -nearPlane * c2, 0 }
            };
        }

        [[nodiscard]]
        static auto orthographic(Size2 viewSize, float nearPlane, float farPlane) noexcept -> Mat4x4 {
            static constexpr auto Two = 2.0f;
            const auto c = farPlane - nearPlane;

            const auto w = static_cast<float>(viewSize.width);
            const auto h = static_cast<float>(viewSize.height);

            return {
                { Two / w,       0,              0, 0 },
                {       0, Two / h,              0, 0 },
                {       0,       0,          1 / c, 0 },
                {       0,       0, -nearPlane / c, 1 }
            };
        }


        [[nodiscard]]
        auto operator[](int i) noexcept -> Vec4& {
            return *(&r1 + i);
        }

        [[nodiscard]]
        auto operator[](int i) const noexcept -> const Vec4& {
            return const_cast<Mat4x4&>(*this)[i];
        }


        [[nodiscard]]
        auto transformNormal(Vec3 norm) const noexcept -> Vec3 {
            // treat the 3x3 submatrix as the rotation/scale component of a transform
            return Vec3::splat(norm.x) * r1.xyz() + Vec3::splat(norm.y) * r2.xyz() + Vec3::splat(norm.z) * r3.xyz();
        }

        [[nodiscard]]
        auto transformCoord(Vec3 coord) const noexcept -> Vec3 {
            // scale and rotate the coordinate point, then add in the translation component
            return transformNormal(coord) + r4.xyz();
        }


        Vec4 r1;
        Vec4 r2;
        Vec4 r3;
        Vec4 r4;
    };

}

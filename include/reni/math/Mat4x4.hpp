#pragma once

#include "MatBase.hpp"
#include "Vec3.hpp"
#include "Vec4.hpp"

#include <cmath>
#include <iterator>

namespace reni {

    struct Mat4x4 : public math::MatBase<Mat4x4, Vec4, 4> {

        friend Vec4* begin(Mat4x4& m) noexcept {
            return &m.r1;
        }

        friend Vec4* end(Mat4x4& m) noexcept {
            return std::next(begin(m), Order);
        }


        static Mat4x4 translation(float dx, float dy, float dz) noexcept {
            return {
                { 1.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f, 0.0f },
                {   dx,   dy,   dz, 1.0f }
            };
        }


        static Mat4x4 perspective(float aspectRatio, float fov, float nearPlane, float farPlane) noexcept {
            const auto c1 = 1.0f / std::tan(fov / 2.0f);
            const auto c2 = farPlane / (farPlane - nearPlane);

            return {
                { c1 / aspectRatio, 0.0f,            0.0f, 0.0f },
                {             0.0f,   c1,            0.0f, 0.0f },
                {             0.0f, 0.0f,              c2, 1.0f },
                {             0.0f, 0.0f, -nearPlane * c2, 0.0f }
            };
        }


        static Mat4x4 orthographic(float width, float height, float nearPlane, float farPlane) noexcept {
            static constexpr auto Two = 2.0f;
            const auto c = farPlane - nearPlane;

            return {
                { Two / width,         0.0f,           0.0f, 0.0f },
                {        0.0f, Two / height,           0.0f, 0.0f },
                {        0.0f,         0.0f,       1.0f / c, 0.0f },
                {        0.0f,         0.0f, -nearPlane / c, 1.0f }
            };
        }


        Mat4x4() noexcept = default;

        Mat4x4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4) noexcept
            : r1(r1), r2(r2), r3(r3), r4(r4) {}


        Vec3 transformNormal(Vec3 norm) const noexcept {
            // treat the 3x3 submatrix as the rotation/scale component of a transform
            return Vec3::splat(norm.x) * r1.xyz() + Vec3::splat(norm.y) * r2.xyz() + Vec3::splat(norm.z) * r3.xyz();
        }


        Vec3 transformCoord(Vec3 coord) const noexcept {
            // scale and rotate the coordinate point, then add in the translation component
            return transformNormal(coord) + r4.xyz();
        }


        Vec4 r1;
        Vec4 r2;
        Vec4 r3;
        Vec4 r4;
    };
}

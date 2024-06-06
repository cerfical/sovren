#pragma once

#include "Mat3x3.hpp"
#include "MatBase.hpp"
#include "Vec4.hpp"
#include "mat_ops.hpp"

#include <array>
#include <cmath>

namespace reni {

    struct Mat4x4 : public math::MatBase<Mat4x4, Mat3x3, Vec4> {

        friend bool operator==(const Mat4x4&, const Mat4x4&) noexcept = default;

        static inline constexpr int Order = 4;


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
            : rows{ r1, r2, r3, r4 } {}


        const Vec4& operator[](int i) const noexcept {
            return const_cast<Mat4x4&>(*this)[i];
        }

        Vec4& operator[](int i) noexcept {
            return rows[i];
        }


        std::array<Vec4, Order> rows;
    };

}

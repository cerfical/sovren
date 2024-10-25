#pragma once

#include "Vec3.hpp"
#include "vec_ops.hpp"

namespace sovren {

    struct Vec4 {

        [[nodiscard]]
        static consteval auto order() noexcept -> int {
            return 4;
        }

        [[nodiscard]]
        static auto splat(float v) noexcept -> Vec4 {
            return fillVector<Vec4>(v);
        }


        [[nodiscard]]
        auto operator[](int i) noexcept -> float& {
            return *(&x + i);
        }

        [[nodiscard]]
        auto operator[](int i) const noexcept -> const float& {
            return const_cast<Vec4&>(*this)[i];
        }


        [[nodiscard]]
        auto xyz() const noexcept -> Vec3 {
            return { .x = x, .y = y, .z = z };
        }

        void setXyz(Vec3 xyz) {
            x = xyz.x;
            y = xyz.y;
            z = xyz.z;
        }


        float x = {};
        float y = {};
        float z = {};
        float w = {};
    };

}

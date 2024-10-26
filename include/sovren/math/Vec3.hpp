#pragma once

#include "Vec2.hpp"
#include "vec_ops.hpp"

namespace sovren {

    struct Vec3 {

        [[nodiscard]]
        static consteval auto order() noexcept -> int {
            return 3;
        }

        [[nodiscard]]
        static auto splat(float v) noexcept -> Vec3 {
            return fillVector<Vec3>(v, 0);
        }


        [[nodiscard]]
        auto operator[](int i) noexcept -> float& {
            return *(&x + i);
        }

        [[nodiscard]]
        auto operator[](int i) const noexcept -> const float& {
            return const_cast<Vec3&>(*this)[i];
        }


        [[nodiscard]]
        auto xy() const noexcept -> Vec2 {
            return { .x = x, .y = y };
        }

        void setXy(Vec2 xy) noexcept {
            x = xy.x;
            y = xy.y;
        }


        float x = {};
        float y = {};
        float z = {};
    };


    [[nodiscard]]
    inline auto cross(Vec3 lhs, Vec3 rhs) noexcept -> Vec3 {
        return {
            .x = (lhs.y * rhs.z) - (lhs.z * rhs.y),
            .y = (lhs.z * rhs.x) - (lhs.x * rhs.z),
            .z = (lhs.x * rhs.y) - (lhs.y * rhs.x)
        };
    }

}

#pragma once

#include "vec_ops.hpp"

namespace sovren {

    struct Vec2 {

        [[nodiscard]]
        static consteval auto order() noexcept -> int {
            return 2;
        }

        [[nodiscard]]
        static auto splat(float v) noexcept -> Vec2 {
            return fillVector<Vec2>(v, 0);
        }


        [[nodiscard]]
        auto operator[](int i) noexcept -> float& {
            return *(&x + i);
        }

        [[nodiscard]]
        auto operator[](int i) const noexcept -> const float& {
            return const_cast<Vec2&>(*this)[i];
        }


        float x = {};
        float y = {};
    };

}

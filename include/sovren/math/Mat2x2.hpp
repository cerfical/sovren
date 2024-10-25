#pragma once

#include "Vec2.hpp"
#include "mat_ops.hpp"

namespace sovren {

    struct Mat2x2 {

        [[nodiscard]]
        static consteval auto order() noexcept -> int {
            return 2;
        }

        [[nodiscard]]
        static auto identity() noexcept -> Mat2x2 {
            return makeIdentityMatrix<Mat2x2>();
        }


        [[nodiscard]]
        auto operator[](int i) noexcept -> Vec2& {
            return *(&r1 + i);
        }

        [[nodiscard]]
        auto operator[](int i) const noexcept -> const Vec2& {
            return const_cast<Mat2x2&>(*this)[i];
        }


        Vec2 r1;
        Vec2 r2;
    };

}

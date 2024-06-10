#pragma once

#include "../math/Vec2.hpp"

#include <compare>

namespace reni {

    /**
     * @brief Dimensions of a rectangular area in 2D space as a pair of two real numbers.
     */
    struct Size2 {

        friend std::partial_ordering operator<=>(Size2, Size2) noexcept = default;


        Size2() noexcept = default;

        Size2(Vec2 vec) noexcept
            : Size2(vec.x, vec.y) {}

        Size2(float width, float height) noexcept
            : width(width), height(height) {}


        operator Vec2() const noexcept {
            return { width, height };
        }


        float width = {};
        float height = {};
    };

}

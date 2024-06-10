#pragma once

#include "../math/Vec3.hpp"

#include <compare>

namespace reni {

    /**
     * @brief Dimensions of a rectangular region in 3D space as a triple of real numbers.
     */
    struct Size3 {

        friend std::partial_ordering operator<=>(Size3, Size3) noexcept = default;


        Size3() noexcept = default;

        Size3(Vec3 vec) noexcept
            : Size3(vec.x, vec.y, vec.z) {}

        Size3(float width, float height, float depth) noexcept
            : width(width), height(height), depth(depth) {}


        operator Vec3() const noexcept {
            return { width, height, depth };
        }


        float width = {};
        float height = {};
        float depth = {};
    };

}

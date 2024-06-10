#pragma once

#include "../math/Vec2.hpp"

#include <compare>

namespace reni {

    /**
     * @brief Displacement of a point in 2D space as a pair of real numbers.
     */
    struct Displace2 {

        friend std::partial_ordering operator<=>(Displace2, Displace2) noexcept = default;


        Displace2() noexcept = default;

        Displace2(Vec2 vec) noexcept
            : Displace2(vec.x, vec.y) {}

        Displace2(float dx, float dy) noexcept
            : dx(dx), dy(dy) {}


        operator Vec2() const noexcept {
            return { dx, dy };
        }


        float dx = {};
        float dy = {};
    };

}

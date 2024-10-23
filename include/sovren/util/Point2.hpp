#pragma once

#include "../math/Vec2.hpp"

#include <compare>

namespace sovren {

    /**
     * @brief Coordinates of a point in 2D space as a pair of real numbers.
     */
    struct Point2 {

        friend std::partial_ordering operator<=>(Point2, Point2) noexcept = default;


        Point2() noexcept = default;

        Point2(Vec2 vec) noexcept
            : Point2(vec.x, vec.y) {}

        Point2(float x, float y) noexcept
            : x(x), y(y) {}


        operator Vec2() const noexcept {
            return { x, y };
        }


        float x = {};
        float y = {};
    };

}

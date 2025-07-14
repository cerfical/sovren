#pragma once

#include <compare>

namespace sovren {

    /**
     * @brief Coordinates of a point in 2D space as a pair of integers.
     */
    struct Point2 {

        friend auto operator<=>(Point2 lhs, Point2 rhs) -> std::strong_ordering = default;


        int x = {}; /**< @brief X-coordinate. */
        int y = {}; /**< @brief Y-coordinate. */
    };

}

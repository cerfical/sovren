#pragma once

#include <compare>

namespace sovren {

    /**
     * @brief Dimensions of a rectangular region in 2D space as a pair of integers.
     */
    struct Size2 {

        friend auto operator<=>(Size2 lhs, Size2 rhs) -> std::strong_ordering = default;


        int width = {};  /**< @brief Width of the rectangle. */
        int height = {}; /**< @brief Height of the rectangle. */
    };

}

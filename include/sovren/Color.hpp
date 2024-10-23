#pragma once

#include <compare>

namespace sovren {

    /**
     * @brief RGBA color in the form of the four real components.
     */
    struct Color {

        friend auto operator<=>(Color lhs, Color rhs) -> std::partial_ordering = default;


        /**
         * @brief Construct a color from the four RGBA components.
         */
        static auto fromRgba(float r, float g, float b, float a) noexcept -> Color {
            return { .red = r, .green = g, .blue = b, .alpha = a };
        }


        /**
         * @brief Construct a color from the three RGB components.
         */
        static auto fromRgb(float r, float g, float b) noexcept -> Color {
            return fromRgba(r, g, b, 1.0);
        }


        float red = {};   /**< @brief Red color component. */
        float green = {}; /**< @brief Green color component. */
        float blue = {};  /**< @brief Blue color component. */
        float alpha = {}; /**< @brief Alpha color component. */
    };

}

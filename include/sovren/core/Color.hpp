#pragma once

#include "math/Vec4.hpp"
#include "math/util.hpp"

#include <compare>
#include <cstdint>

namespace sovren {

    /**
     * @brief RGBA color in the form of the four integer components.
     */
    struct Color {

        friend auto operator<=>(Color lhs, Color rhs) -> std::partial_ordering = default;


        static constexpr std::uint8_t MinComponentValue = 0;   /**< @brief Minimum value of a color component. */
        static constexpr std::uint8_t MaxComponentValue = 255; /**< @brief Maximum value of a color component. */


        /**
         * @brief Construct a color from the four RGBA components.
         */
        static auto fromRgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept -> Color {
            return { .r = r, .g = g, .b = b, .a = a };
        }


        /**
         * @brief Construct a color from the three RGB components.
         */
        static auto fromRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept -> Color {
            return fromRgba(r, g, b, MaxComponentValue);
        }


        /**
         * @brief Construct a color from the RGBA represented as a real-valued vector.
         */
        static auto fromVec(Vec4 rgba) noexcept -> Color {
            return fromRgba(
                static_cast<std::uint8_t>(clamp(rgba.x, 0, 1) * MaxComponentValue),
                static_cast<std::uint8_t>(clamp(rgba.y, 0, 1) * MaxComponentValue),
                static_cast<std::uint8_t>(clamp(rgba.z, 0, 1) * MaxComponentValue),
                static_cast<std::uint8_t>(clamp(rgba.w, 0, 1) * MaxComponentValue)
            );
        }


        /**
         * @brief Construct a white color.
         */
        static auto white() noexcept -> Color {
            return fromRgb(MaxComponentValue, MaxComponentValue, MaxComponentValue);
        }


        /**
         * @brief Construct a black color.
         */
        static auto black() noexcept -> Color {
            return fromRgb(MinComponentValue, MinComponentValue, MinComponentValue);
        }


        /**
         * @brief Present the color as a real-valued vector.
         */
        [[nodiscard]]
        auto toVec() const noexcept -> Vec4 {
            return {
                static_cast<float>(r) / MaxComponentValue,
                static_cast<float>(g) / MaxComponentValue,
                static_cast<float>(b) / MaxComponentValue,
                static_cast<float>(a) / MaxComponentValue,
            };
        }


        std::uint8_t r = {}; /**< @brief Red color component. */
        std::uint8_t g = {}; /**< @brief Green color component. */
        std::uint8_t b = {}; /**< @brief Blue color component. */
        std::uint8_t a = {}; /**< @brief Alpha color component. */
    };

}

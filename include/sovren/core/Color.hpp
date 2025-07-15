#pragma once

#include "concepts.hpp"

#include <compare>
#include <concepts>
#include <cstdint>

namespace sovren {

    /**
     * @brief RGBA color with integer components.
     */
    struct Color {

        friend auto operator<=>(Color, Color) -> std::strong_ordering = default;


        static auto fromRgba(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept -> Color {
            return { r, g, b, a };
        }

        static auto fromRgb(std::uint8_t r, std::uint8_t g, std::uint8_t b) noexcept -> Color {
            return fromRgba(r, g, b, 255);
        }


        static auto white() noexcept -> Color {
            return fromRgb(255, 255, 255);
        }

        static auto black() noexcept -> Color {
            return fromRgb(0, 0, 0);
        }


        /**
         * @brief Decompose the color into a 4-element aggregate.
         */
        template <AggregateN<4> T>
        [[nodiscard]]
        explicit operator T() const noexcept {
            auto [v1, v2, v3, v4] = T{};

            v1 = static_cast<decltype(v1)>(r);
            v2 = static_cast<decltype(v2)>(g);
            v3 = static_cast<decltype(v3)>(b);
            v4 = static_cast<decltype(v4)>(a);

            if constexpr(std::floating_point<decltype(v1)>) {
                v1 /= 255;
            }
            if constexpr(std::floating_point<decltype(v2)>) {
                v2 /= 255;
            }
            if constexpr(std::floating_point<decltype(v3)>) {
                v3 /= 255;
            }
            if constexpr(std::floating_point<decltype(v4)>) {
                v4 /= 255;
            }

            return T{ v1, v2, v3, v4 };
        }

        std::uint8_t r = {};
        std::uint8_t g = {};
        std::uint8_t b = {};
        std::uint8_t a = {};
    };

}

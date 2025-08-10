#pragma once

#include <sovren/util/AggregateN.hpp>

#include <compare>
#include <concepts>
#include <cstdint>
#include <format>

namespace sovren {

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
            const auto [v1, v2, v3, v4] = T{};
            return T{
                scaleValue<decltype(v1)>(r),
                scaleValue<decltype(v2)>(g),
                scaleValue<decltype(v3)>(b),
                scaleValue<decltype(v4)>(a),
            };
        }

        std::uint8_t r = {};
        std::uint8_t g = {};
        std::uint8_t b = {};
        std::uint8_t a = {};

    private:
        template <typename To>
        static constexpr auto scaleValue(std::uint8_t v) noexcept -> To {
            if constexpr(std::floating_point<To>) {
                return static_cast<To>(v) / 255;
            }
            return static_cast<To>(v);
        }
    };

}

template <>
struct std::formatter<sovren::Color> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::Color c, format_context& ctx) const {
        if(c.a == 255) {
            return format_to(ctx.out(), "rgb({}, {}, {})", c.r, c.g, c.b);
        }
        return format_to(ctx.out(), "rgba({}, {}, {}, {})", c.r, c.g, c.b, c.a);
    }
};

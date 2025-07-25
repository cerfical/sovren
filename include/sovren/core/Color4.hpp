#pragma once

#include "AggregateN.hpp"

#include <concepts>
#include <cstdint>
#include <limits>

namespace sovren {

    template <typename T>
        requires std::integral<T> || std::floating_point<T>
    struct Color4 {
        friend auto operator<=>(Color4, Color4) = default;

        static auto fromRgba(T r, T g, T b, T a) noexcept -> Color4 {
            return { r, g, b, a };
        }

        static auto fromRgb(T r, T g, T b) noexcept -> Color4 {
            return fromRgba(r, g, b, maxValue());
        }

        static auto white() noexcept -> Color4 {
            return fromRgb(maxValue(), maxValue(), maxValue());
        }

        static auto black() noexcept -> Color4 {
            return fromRgb(0, 0, 0);
        }

        /**
         * @brief Decompose the color into a 4-element aggregate.
         */
        template <AggregateN<4> U>
        [[nodiscard]]
        explicit operator U() const noexcept {
            const auto [v1, v2, v3, v4] = U{};
            return U{
                scaleValue<decltype(v1)>(r),
                scaleValue<decltype(v2)>(g),
                scaleValue<decltype(v3)>(b),
                scaleValue<decltype(v4)>(a),
            };
        }

        T r = {};
        T g = {};
        T b = {};
        T a = {};

    private:
        template <typename U = T>
        static constexpr auto maxValue() noexcept -> U {
            if constexpr(std::floating_point<U>) {
                return 1.0;
            }
            return std::numeric_limits<U>::max();
        }

        template <typename To, typename From>
        static constexpr auto scaleValue(From v) noexcept -> To {
            if constexpr(std::floating_point<From> && std::integral<To>) {
                return static_cast<To>(v * maxValue<To>());
            }
            if constexpr(std::integral<From> && std::floating_point<To>) {
                return static_cast<To>(static_cast<To>(v) / maxValue<From>());
            }
            return static_cast<To>(v);
        }
    };

    using Color4f = Color4<float>;
    using Color4i = Color4<std::uint8_t>;

}

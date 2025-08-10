#pragma once

#include <sovren/util/AggregateN.hpp>

#include <concepts>
#include <format>

namespace sovren {

    template <typename T>
        requires std::floating_point<T> || std::integral<T>
    struct Size2 {
        friend auto operator<=>(Size2, Size2) = default;

        /**
         * @brief Decompose the size into a 2-element aggregate.
         */
        template <AggregateN<2> U>
        [[nodiscard]]
        explicit operator U() const noexcept {
            const auto [v1, v2] = U{};
            return U{
                static_cast<decltype(v1)>(width),
                static_cast<decltype(v2)>(height),
            };
        }

        T width = {};
        T height = {};
    };

    using Size2i = Size2<int>;
    using Size2f = Size2<float>;

}

template <typename T>
struct std::formatter<sovren::Size2<T>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::Size2<T> s, format_context& ctx) const {
        return format_to(ctx.out(), "{}x{}", s.width, s.height);
    }
};

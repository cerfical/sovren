#pragma once

#include "AggregateN.hpp"
#include "Point2.hpp"
#include "Size2.hpp"

#include <concepts>
#include <format>

namespace sovren {

    template <typename T>
        requires std::floating_point<T> || std::integral<T>
    struct Rect2 {
        friend auto operator<=>(Rect2, Rect2) = default;

        /**
         * @brief Decompose the rectangle into a 2-element aggregate.
         */
        template <AggregateN<2> U>
        [[nodiscard]]
        explicit operator U() const noexcept {
            const auto [v1, v2] = U{};
            return U{
                static_cast<decltype(v1)>(topLeft),
                static_cast<decltype(v2)>(size),
            };
        }

        Point2<T> topLeft;
        Size2<T> size;
    };

    using Rect2f = Rect2<float>;
    using Rect2i = Rect2<int>;

}

template <typename T>
struct std::formatter<sovren::Rect2<T>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::Rect2<T> r, format_context& ctx) const {
        return format_to(ctx.out(), "{{{}, {}}}", r.topLeft, r.size);
    }
};

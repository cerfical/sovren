#pragma once

#include "AggregateN.hpp"
#include "Point2.hpp"

#include <concepts>
#include <format>

namespace sovren {

    template <typename T>
        requires std::floating_point<T> || std::integral<T>
    struct Line2 {
        friend auto operator<=>(Line2, Line2) = default;

        /**
         * @brief Decompose the line into a 2-element aggregate.
         */
        template <AggregateN<2> U>
        [[nodiscard]]
        explicit operator U() const noexcept {
            const auto [v1, v2] = U{};
            return U{
                static_cast<decltype(v1)>(start),
                static_cast<decltype(v2)>(end),
            };
        }

        Point2<T> start;
        Point2<T> end;
    };

    using Line2f = Line2<float>;
    using Line2i = Line2<int>;

}

template <typename T>
struct std::formatter<sovren::Line2<T>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::Line2<T> l, format_context& ctx) const {
        return format_to(ctx.out(), "{{{}, {}}}", l.start, l.end);
    }
};

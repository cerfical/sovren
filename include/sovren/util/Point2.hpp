#pragma once

#include <sovren/util/AggregateN.hpp>

#include <concepts>
#include <format>

namespace sovren {

    template <typename T>
        requires std::floating_point<T> || std::integral<T>
    struct Point2 {
        friend auto operator<=>(Point2, Point2) = default;

        /**
         * @brief Decompose the point into a 2-element aggregate.
         */
        template <AggregateN<2> U>
        [[nodiscard]]
        explicit operator U() const noexcept {
            const auto [v1, v2] = U{};
            return U{
                static_cast<decltype(v1)>(x),
                static_cast<decltype(v2)>(y),
            };
        }

        T x = {};
        T y = {};
    };

    using Point2i = Point2<int>;
    using Point2f = Point2<float>;

}

template <typename T>
struct std::formatter<sovren::Point2<T>> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::Point2<T> p, format_context& ctx) const {
        return format_to(ctx.out(), "({}, {})", p.x, p.y);
    }
};

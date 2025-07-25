#pragma once

#include "AggregateN.hpp"
#include "Point2.hpp"
#include "Size2.hpp"

namespace sovren {

    template <typename T>
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

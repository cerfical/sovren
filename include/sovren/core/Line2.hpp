#pragma once

#include "AggregateN.hpp"
#include "Point2.hpp"

namespace sovren {

    template <typename T>
    struct Line {
        friend auto operator<=>(Line, Line) = default;

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

    using Line2f = Line<float>;
    using Line2i = Line<int>;

}

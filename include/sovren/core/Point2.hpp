#pragma once

#include "concepts.hpp"

namespace sovren {

    template <typename T>
    struct Point2 {

        friend auto operator<=>(Point2 lhs, Point2 rhs) -> auto = default;


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

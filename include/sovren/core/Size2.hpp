#pragma once

#include "concepts.hpp"

namespace sovren {

    template <typename T>
    struct Size2 {

        friend auto operator<=>(Size2 lhs, Size2 rhs) -> auto = default;


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

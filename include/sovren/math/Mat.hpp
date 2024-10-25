#pragma once

#include "Vec.hpp"

#include <concepts>

namespace sovren {

    template <typename M>
    concept Mat = requires(M m) {
        { M::order() } -> std::convertible_to<int>;

        requires Vec<std::decay_t<decltype(m[0])>>;
        requires Vec<std::decay_t<decltype(static_cast<const M&>(m)[0])>>;

        { &m[0] };
        { &static_cast<const M&>(m)[0] };
    };

}

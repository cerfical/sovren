#pragma once

#include <concepts>

namespace sovren {

    template <typename V>
    concept Vec = requires(V v) {
        { V::order() } -> std::convertible_to<int>;

        requires std::convertible_to<std::decay_t<decltype(v[0])>, float>;
        requires std::convertible_to<std::decay_t<decltype(static_cast<const V&>(v)[0])>, float>;

        { &v[0] };
        { &static_cast<const V&>(v)[0] };
    };

}

#pragma once

#include "ConstructibleFromN.hpp"

#include <cstddef>
#include <type_traits>

namespace sovren {

    /**
     * @brief An aggregate type with exactly `N` elements.
     */
    template <typename T, std::size_t N>
    concept AggregateN = std::is_aggregate_v<T>
        && ConstructibleFromN<T, N>
        && !ConstructibleFromN<T, N + 1>;

}

#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace sovren {

    namespace detail {
        struct ConvertibleToAny {
            template <typename T>
            constexpr operator T() const noexcept;
        };
    }


    /**
     * @brief A type that can be constructed from `N` values.
     */
    template <typename T, std::size_t N>
    concept ConstructibleFromN = []<std::size_t... Is>(std::index_sequence<Is...> /*unused*/) noexcept {
        return requires {
            T{ (static_cast<void>(Is), std::declval<detail::ConvertibleToAny>())... };
        };
    }(std::make_index_sequence<N>{});


    /**
     * @brief An aggregate type with exactly `N` elements.
     */
    template <typename T, std::size_t N>
    concept AggregateN = std::is_aggregate_v<T>
        && ConstructibleFromN<T, N>
        && !ConstructibleFromN<T, N + 1>;

}

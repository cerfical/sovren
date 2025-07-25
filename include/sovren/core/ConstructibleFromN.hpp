#pragma once

#include <cstddef>
#include <utility>

namespace sovren {
    namespace impl {
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
            T{ (static_cast<void>(Is), std::declval<impl::ConvertibleToAny>())... };
        };
    }(std::make_index_sequence<N>{});

}

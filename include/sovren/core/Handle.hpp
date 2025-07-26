#pragma once

#include <concepts>
#include <format>

namespace sovren {

    /**
     * @brief A simple type to represent opaque resources.
     */
    class Handle {
    public:
        Handle() = default;

        template <std::convertible_to<void*> T>
        Handle(T handle) noexcept
            : handle_(static_cast<void*>(handle)) {}

        template <typename T>
        [[nodiscard]]
        explicit operator T() const noexcept {
            return static_cast<T>(handle_);
        }

    private:
        void* handle_ = {};
    };

}

template <>
struct std::formatter<sovren::Handle> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::Handle h, format_context& ctx) const {
        return format_to(ctx.out(), "{}", static_cast<void*>(h));
    }
};

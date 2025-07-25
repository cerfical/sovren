#pragma once

#include <concepts>

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

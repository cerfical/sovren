#pragma once

namespace sovren {

    /**
     * @brief Opaque type to manage platform-specific window handles.
     */
    class WindowHandle {
    public:

        WindowHandle() = default;


        /**
         * @brief Construct a new handle, erasing the handle type information.
         */
        template <typename T>
        WindowHandle(T handle) noexcept
            : handle_(static_cast<void*>(handle)) {}


        /**
         * @brief Convert the handle to the desired format.
         */
        template <typename T>
        auto as() const noexcept -> T {
            return static_cast<T>(handle_);
        }


        /**
         * @brief Get the handle as a raw untyped pointer.
         */
        [[nodiscard]]
        auto get() const noexcept -> void* {
            return handle_;
        }


    private:
        void* handle_ = {};
    };

}

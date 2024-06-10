#pragma once

#include "LogKind.hpp"
#include "LogMessage.hpp"
#include "LogTag.hpp"

#include <concepts>
#include <format>
#include <source_location>
#include <string_view>
#include <utility>

namespace reni {

    /**
     * @brief Helper class to provide basic formatting and location information to log messages.
     */
    template <typename... Args>
    class LogMessageFormat {
    public:

        /**
         * @brief Construct a message to be formatted from an arbitrary string sequence.
         */
        template <std::convertible_to<std::string_view> T>
        consteval LogMessageFormat(T&& message, const std::source_location& loc = std::source_location::current())
            : message_(std::forward<T>(message)), location_(loc) {}

        /**
         * @brief Format and compose a message from the provided arguments.
         */
        LogMessage makeMessage(LogTag tag, LogKind kind, Args&&... args) const {
            return LogMessage(
                kind,
                std::move(tag),
                std::vformat(message_.get(), std::make_format_args(std::forward<Args>(args)...)),
                location_
            );
        }

    private:
        std::format_string<Args...> message_;
        std::source_location location_;
    };

}

#pragma once

#include "LogMessage.hpp"

#include <concepts>
#include <format>
#include <string_view>
#include <utility>

namespace sovren {

    template <typename... Args>
    class LogMessageFormat {
    public:

        template <std::convertible_to<std::string_view> T>
        consteval LogMessageFormat(T&& msg, const std::source_location& loc = std::source_location::current())
            : msg_(std::forward<T>(msg)), loc_(loc) {}


        auto makeMessage(LogKind kind, Args&&... args) const -> LogMessage {
            return {
                .desc = std::vformat(msg_.get(), std::make_format_args(args...)),
                .loc = loc_,
                .kind = kind
            };
        }

    private:
        std::format_string<Args...> msg_;
        std::source_location loc_;
    };

}

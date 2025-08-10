#pragma once

#include <format>
#include <string_view>

namespace sovren::log {

    enum class Level {
        None,
        Error,
        Warn,
        Info,
        Debug
    };

}

template <>
struct std::formatter<sovren::log::Level> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::log::Level kind, format_context& ctx) const {
        using namespace sovren::log;

        string_view str;
        switch(kind) {
            case Level::None:  str = "None"; break;
            case Level::Debug: str = "Debug"; break;
            case Level::Info:  str = "Info"; break;
            case Level::Warn:  str = "Warn"; break;
            case Level::Error: str = "Error"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

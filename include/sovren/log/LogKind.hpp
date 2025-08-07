#pragma once

#include <format>
#include <string_view>

namespace sovren {

    enum class LogKind {
        None,
        Debug,
        Info,
        Warn,
        Error
    };

}

template <>
struct std::formatter<sovren::LogKind> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::LogKind kind, format_context& ctx) const {
        using namespace sovren;

        string_view str;
        switch(kind) {
            case LogKind::None:  str = "None"; break;
            case LogKind::Debug: str = "Debug"; break;
            case LogKind::Info:  str = "Info"; break;
            case LogKind::Warn:  str = "Warn"; break;
            case LogKind::Error: str = "Error"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

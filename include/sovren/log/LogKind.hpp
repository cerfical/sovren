#pragma once

#include <format>

namespace sovren {

    enum class LogKind {
        Debug,
        Info,
        Warning,
        Error
    };

}

template <>
struct std::formatter<sovren::LogKind> {
    static constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    static auto format(sovren::LogKind kind, std::format_context& ctx) {
        std::string_view str;
        switch(kind) {
            case sovren::LogKind::Debug:   str = "Debug"; break;
            case sovren::LogKind::Info:    str = "Info"; break;
            case sovren::LogKind::Warning: str = "Warning"; break;
            case sovren::LogKind::Error:   str = "Error"; break;
        }
        return std::format_to(ctx.out(), "{}", str);
    }
};

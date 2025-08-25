#pragma once

#include <format>
#include <string_view>

namespace sovren {

    enum class WindowEventType {
        None,
        Resize,
        Close,
    };

}

template <>
struct std::formatter<sovren::WindowEventType> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::WindowEventType type, format_context& ctx) const {
        using namespace sovren;

        string_view str;
        switch(type) {
            case WindowEventType::None:   str = "None"; break;
            case WindowEventType::Resize: str = "Resize"; break;
            case WindowEventType::Close:  str = "Close"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

#pragma once

#include <format>
#include <string_view>

namespace sovren {

    enum class MouseEventType {
        None,
        Move,
        ButtonDown,
        ButtonUp,
    };

}

template <>
struct std::formatter<sovren::MouseEventType> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::MouseEventType type, format_context& ctx) const {
        using namespace sovren;

        string_view str;
        switch(type) {
            case MouseEventType::None:       str = "None"; break;
            case MouseEventType::Move:       str = "Move"; break;
            case MouseEventType::ButtonDown: str = "ButtonDown"; break;
            case MouseEventType::ButtonUp:   str = "ButtonUp"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

#pragma once

#include <format>
#include <string_view>

namespace sovren {

    enum class KeyEventType {
        None,
        KeyDown,
        KeyUp,
    };

}

template <>
struct std::formatter<sovren::KeyEventType> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::KeyEventType type, format_context& ctx) const {
        using namespace sovren;

        string_view str;
        switch(type) {
            case KeyEventType::None:    str = "None"; break;
            case KeyEventType::KeyDown: str = "KeyDown"; break;
            case KeyEventType::KeyUp:   str = "KeyUp"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

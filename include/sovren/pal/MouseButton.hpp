#pragma once

#include <format>
#include <string_view>

namespace sovren::pal {

    enum class MouseButton {
        None,
        Left,
        Middle,
        Right,
    };

}

template <>
struct std::formatter<sovren::pal::MouseButton> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::pal::MouseButton b, format_context& ctx) const {
        using namespace sovren::pal;

        string_view str;
        switch(b) {
            case MouseButton::None:   str = "None"; break;
            case MouseButton::Left:   str = "Left"; break;
            case MouseButton::Middle: str = "Middle"; break;
            case MouseButton::Right:  str = "Right"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

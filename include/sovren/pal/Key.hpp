#pragma once

#include <format>
#include <string_view>

namespace sovren::pal {

    enum class Key {
        None,
        LeftArrow,
        RightArrow,
        UpArrow,
        DownArrow,
    };

}

template <>
struct std::formatter<sovren::pal::Key> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::pal::Key k, format_context& ctx) const {
        using namespace sovren::pal;

        string_view str;
        switch(k) {
            case Key::None:       str = "None"; break;
            case Key::LeftArrow:  str = "LeftArrow"; break;
            case Key::RightArrow: str = "RightArrow"; break;
            case Key::UpArrow:    str = "UpArrow"; break;
            case Key::DownArrow:  str = "DownArrow"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

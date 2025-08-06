#pragma once

#include <format>
#include <string>

namespace sovren::rhi {

    enum class Format {
        None,

        Float3,
    };

}

template <>
struct std::formatter<sovren::rhi::Format> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::rhi::Format fmt, format_context& ctx) const {
        using namespace sovren::rhi;

        std::string str;
        switch(fmt) {
            case Format::None: {
                str = "None";
                break;
            }
            case Format::Float3: {
                str = "Float3";
                break;
            }
        }
        return format_to(ctx.out(), "{}", str);
    }
};

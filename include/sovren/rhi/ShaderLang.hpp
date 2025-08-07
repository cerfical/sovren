#pragma once

#include <format>
#include <string_view>

namespace sovren::rhi {

    enum class ShaderLang {
        None,
        CompiledHLSL,
    };

}

template <>
struct std::formatter<sovren::rhi::ShaderLang> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::rhi::ShaderLang fmt, format_context& ctx) const {
        using namespace sovren::rhi;

        string_view str;
        switch(fmt) {
            case ShaderLang::None:         str = "None"; break;
            case ShaderLang::CompiledHLSL: str = "CompiledHLSL"; break;
        }
        return format_to(ctx.out(), "{}", str);
    }
};

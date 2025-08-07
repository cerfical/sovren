#pragma once

#include <format>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <vector>

namespace sovren::rhi {

    enum class ShaderStages {
        None = 0,
        Vertex = 0b0001,
        Pixel = 0b0010,
    };

    inline auto operator|(ShaderStages lhs, ShaderStages rhs) noexcept -> ShaderStages {
        using IntType = std::underlying_type_t<ShaderStages>;
        return static_cast<ShaderStages>(
            static_cast<IntType>(lhs) | static_cast<IntType>(rhs)
        );
    }

    inline auto operator&(ShaderStages lhs, ShaderStages rhs) noexcept -> ShaderStages {
        using IntType = std::underlying_type_t<ShaderStages>;
        return static_cast<ShaderStages>(
            static_cast<IntType>(lhs) & static_cast<IntType>(rhs)
        );
    }

}

template <>
struct std::formatter<sovren::rhi::ShaderStages> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::rhi::ShaderStages stages, format_context& ctx) const {
        using namespace sovren::rhi;

        vector<string_view> strs;
        if(stages == ShaderStages()) {
            strs.emplace_back("None");
        }
        if((stages & ShaderStages::Vertex) != ShaderStages()) {
            strs.emplace_back("Vertex");
        }
        if((stages & ShaderStages::Pixel) != ShaderStages()) {
            strs.emplace_back("Pixel");
        }

        const auto str = strs | views::join_with('|') | ranges::to<string>();
        return format_to(ctx.out(), "{}", str);
    }
};

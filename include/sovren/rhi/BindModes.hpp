#pragma once

#include <format>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <vector>

namespace sovren::rhi {

    enum class BindModes {
        None = 0,
        VertexBuffer = 0b0001,
        IndexBuffer = 0b0010,
        ConstantBuffer = 0b0100,
    };

    inline auto operator|(BindModes lhs, BindModes rhs) noexcept -> BindModes {
        using IntType = std::underlying_type_t<BindModes>;
        return static_cast<BindModes>(
            static_cast<IntType>(lhs) | static_cast<IntType>(rhs)
        );
    }

    inline auto operator&(BindModes lhs, BindModes rhs) noexcept -> BindModes {
        using IntType = std::underlying_type_t<BindModes>;
        return static_cast<BindModes>(
            static_cast<IntType>(lhs) & static_cast<IntType>(rhs)
        );
    }

}

template <>
struct std::formatter<sovren::rhi::BindModes> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::rhi::BindModes bind, format_context& ctx) const {
        using namespace sovren::rhi;

        vector<string_view> strs;
        if(bind == BindModes()) {
            strs.emplace_back("None");
        }
        if((bind & BindModes::VertexBuffer) != BindModes()) {
            strs.emplace_back("VertexBuffer");
        }
        if((bind & BindModes::IndexBuffer) != BindModes()) {
            strs.emplace_back("IndexBuffer");
        }
        if((bind & BindModes::ConstantBuffer) != BindModes()) {
            strs.emplace_back("ConstantBuffer");
        }

        const auto str = strs | views::join_with('|') | ranges::to<string>();
        return format_to(ctx.out(), "{}", str);
    }
};

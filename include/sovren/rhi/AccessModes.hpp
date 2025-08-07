#pragma once

#include <format>
#include <ranges>
#include <string_view>
#include <type_traits>
#include <vector>

namespace sovren::rhi {

    enum class AccessModes {
        None = 0,
        Read = 0b0001,
        Write = 0b0010,
    };

    inline auto operator|(AccessModes lhs, AccessModes rhs) noexcept -> AccessModes {
        using IntType = std::underlying_type_t<AccessModes>;
        return static_cast<AccessModes>(
            static_cast<IntType>(lhs) | static_cast<IntType>(rhs)
        );
    }

    inline auto operator&(AccessModes lhs, AccessModes rhs) noexcept -> AccessModes {
        using IntType = std::underlying_type_t<AccessModes>;
        return static_cast<AccessModes>(
            static_cast<IntType>(lhs) & static_cast<IntType>(rhs)
        );
    }

}

template <>
struct std::formatter<sovren::rhi::AccessModes> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::rhi::AccessModes access, format_context& ctx) const {
        using namespace sovren::rhi;

        vector<string_view> strs;
        if(access == AccessModes()) {
            strs.emplace_back("None");
        }
        if((access & AccessModes::Read) != AccessModes()) {
            strs.emplace_back("Read");
        }
        if((access & AccessModes::Write) != AccessModes()) {
            strs.emplace_back("Write");
        }

        const auto str = strs | views::join_with('|') | ranges::to<string>();
        return format_to(ctx.out(), "{}", str);
    }
};

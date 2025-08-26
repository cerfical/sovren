#pragma once

#include <algorithm>
#include <format>
#include <initializer_list>
#include <ranges>
#include <string>
#include <vector>

namespace sovren::log {

    class Tags {
    public:
        [[nodiscard]]
        friend auto operator+(const Tags& lhs, const Tags& rhs) -> Tags {
            auto tags = lhs;
            tags.tags_.insert(tags.tags_.end(), rhs.tags_.begin(), rhs.tags_.end());
            return tags;
        }

        Tags() = default;

        Tags(std::initializer_list<std::string> tags)
            : tags_(tags) {}

        Tags(const Tags&) = default;
        auto operator=(const Tags&) -> Tags& = default;

        Tags(Tags&&) = default;
        auto operator=(Tags&&) -> Tags& = default;

        ~Tags() = default;

        [[nodiscard]]
        auto str() const -> std::string {
            return tags_
                | std::views::transform([](const auto& tag) {
                       return std::format("[{}]", tag);
                   })
                | std::views::join
                | std::ranges::to<std::string>();
        }

        [[nodiscard]]
        auto contains(std::string_view tag) const noexcept -> bool {
            return std::ranges::contains(tags_, tag);
        }

        [[nodiscard]]
        auto isEmpty() const noexcept -> bool {
            return tags_.empty();
        }

        [[nodiscard]]
        operator bool() const noexcept {
            return !isEmpty();
        }

    private:
        std::vector<std::string> tags_;
    };

}

template <>
struct std::formatter<sovren::log::Tags> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::log::Tags tags, format_context& ctx) const {
        return format_to(ctx.out(), "{}", tags.str());
    }
};

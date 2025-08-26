#pragma once

#include <sovren/log/Level.hpp>
#include <sovren/log/Tags.hpp>

#include <chrono>
#include <filesystem>
#include <format>
#include <source_location>
#include <string>

namespace sovren::log {

    struct Message {
        std::string desc;
        std::source_location loc;
        Tags tags;
        Level level = {};
    };

}

template <>
struct std::formatter<sovren::log::Message> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(sovren::log::Message msg, format_context& ctx) const {
        const auto time = chrono::current_zone()->to_local(chrono::system_clock::now());
        const auto tags = stringifyTags(msg);

        return format_to(ctx.out(), "[{}][{}]{}: {}", time, msg.level, tags, msg.desc);
    }

private:
    [[nodiscard]]
    static auto stringifyTags(const sovren::log::Message& msg) -> string {
        if(msg.tags) {
            return msg.tags.str();
        }

        // hide source file information for non-debug messages
        if(msg.level < sovren::log::Level::Debug) {
            return "";
        }

        // use the source file information as the tag
        const auto srcFile = filesystem::path(msg.loc.file_name()).filename().string();
        return std::format("{}:{}:{}", srcFile, msg.loc.column(), msg.loc.line());
    }
};

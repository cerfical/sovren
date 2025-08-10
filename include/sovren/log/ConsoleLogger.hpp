#pragma once

#include "LogMessage.hpp"
#include "Logger.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <ranges>

namespace sovren {

    class ConsoleLogger : public Logger {
    public:
        void write(const LogMessage& msg) override {
            const auto local = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
            const auto tags = extractTags(msg);

            std::clog << std::format("[{}][{}]{}: {}\n", local, msg.kind, tags, msg.desc);
        }

    private:
        static auto extractTags(const LogMessage& msg) -> std::string {
            if(!msg.tags.empty()) {
                return stringifyTags(msg.tags);
            }

            // use the source file information as the only tag
            const auto srcFile = std::filesystem::path(msg.loc.file_name()).filename().string();
            const auto tag = std::format("{}:{}:{}", srcFile, msg.loc.column(), msg.loc.line());

            return stringifyTags(std::views::single(tag));
        }

        template <std::ranges::range R>
        static auto stringifyTags(R&& tags) -> std::string {
            return std::forward<R>(tags) | std::views::transform([](const auto& tag) {
                       return std::format("[{}]", tag);
                   })
                | std::views::join
                | std::ranges::to<std::string>();
        }
    };

}

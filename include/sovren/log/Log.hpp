#pragma once

#include "Logger.hpp"

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace sovren {

    class Log {
        template <typename... Args>
        class FormatImpl;

        template <typename... Args>
        using Format = FormatImpl<std::type_identity_t<Args>...>;

    public:
        Log() = default;

        Log(std::shared_ptr<Logger> logger, std::vector<std::string> tags = {}) noexcept
            : logger_(std::move(logger)), tags_(std::move(tags)) {}

        Log(const Log&) = delete;
        auto operator=(const Log&) -> Log& = delete;

        Log(Log&&) = default;
        auto operator=(Log&&) -> Log& = default;

        ~Log() = default;

        template <typename... Args>
        void debug(const Format<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Debug, msg, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void info(const Format<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Info, msg, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void error(const Format<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Error, msg, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void warn(const Format<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Warn, msg, std::forward<Args>(args)...);
        }

        template <std::convertible_to<std::string>... Tags>
        [[nodiscard]]
        auto withTags(Tags&&... tags) const -> Log {
            // create a new log with replaced tags
            return { logger_, { std::forward<Tags>(tags)... } };
        }

        [[nodiscard]]
        auto tags() const noexcept -> const std::vector<std::string>& {
            return tags_;
        }

        [[nodiscard]]
        auto logger() const noexcept -> std::shared_ptr<Logger> {
            return logger_;
        }

    private:
        template <typename... Args>
        class FormatImpl {
        public:
            template <std::convertible_to<std::string_view> T>
            consteval FormatImpl(T&& msg, const std::source_location& loc = std::source_location::current())
                : msg_(std::forward<T>(msg)), loc_(loc) {}

            auto makeMessage(LogKind kind, std::vector<std::string> tags, Args&&... args) const -> LogMessage {
                return {
                    .desc = std::vformat(msg_.get(), std::make_format_args(args...)),
                    .loc = loc_,
                    .tags = std::move(tags),
                    .kind = kind
                };
            }

        private:
            std::format_string<Args...> msg_;
            std::source_location loc_;
        };

        template <typename... Args>
        void log(LogKind kind, const Format<Args...>& msg, Args&&... args) noexcept {
            if(!logger_) {
                return;
            }

            try {
                logger_->write(msg.makeMessage(kind, tags_, std::forward<Args>(args)...));
            } catch(...) {
                // eat up all logging failures
            }
        }

        std::shared_ptr<Logger> logger_;
        std::vector<std::string> tags_;
    };

}

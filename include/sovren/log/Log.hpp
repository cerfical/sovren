#pragma once

#include <sovren/log/Level.hpp>
#include <sovren/log/Logger.hpp>
#include <sovren/log/Tags.hpp>

#include <sovren/config.hpp>

#include <memory>
#include <string>
#include <type_traits>

namespace sovren::log {

    class Log {
        template <typename... Args>
        class FormatImpl;

        template <typename... Args>
        using Format = FormatImpl<std::type_identity_t<Args>...>;

    public:
        Log() = default;

        Log(
            std::shared_ptr<Logger> logger,
            Level lvl = Config::isDebug() ? Level::Debug : Level::Info,
            Tags tags = {}
        ) noexcept
            : logger_(std::move(logger)), level_(lvl), tags_(std::move(tags)) {}

        Log(const Log&) = delete;
        auto operator=(const Log&) -> Log& = delete;

        Log(Log&&) = default;
        auto operator=(Log&&) -> Log& = default;

        ~Log() = default;

        template <typename... Args>
        void debug(const Format<Args...>& msg, Args&&... args) noexcept {
            log(Level::Debug, msg, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void info(const Format<Args...>& msg, Args&&... args) noexcept {
            log(Level::Info, msg, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void error(const Format<Args...>& msg, Args&&... args) noexcept {
            log(Level::Error, msg, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void warn(const Format<Args...>& msg, Args&&... args) noexcept {
            log(Level::Warn, msg, std::forward<Args>(args)...);
        }

        template <std::convertible_to<std::string>... Tags>
        [[nodiscard]]
        auto withTags(Tags&&... tags) const -> Log {
            // create a new log with replaced tags
            return { logger_, level_, { std::forward<Tags>(tags)... } };
        }

        [[nodiscard]]
        auto tags() const noexcept -> const Tags& {
            return tags_;
        }

        [[nodiscard]]
        auto withLevel(Level lvl) const -> Log {
            return { logger_, lvl, tags_ };
        }

        [[nodiscard]]
        auto level() const noexcept -> Level {
            return level_;
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

            auto makeMessage(Level lvl, Tags tags, Args&&... args) const -> Message {
                return {
                    .desc = std::vformat(msg_.get(), std::make_format_args(args...)),
                    .loc = loc_,
                    .tags = std::move(tags),
                    .level = lvl
                };
            }

        private:
            std::format_string<Args...> msg_;
            std::source_location loc_;
        };

        template <typename... Args>
        void log(Level lvl, const Format<Args...>& msg, Args&&... args) noexcept {
            if(!logger_ || lvl > level_) {
                return;
            }

            try {
                logger_->write(msg.makeMessage(lvl, tags_, std::forward<Args>(args)...));
            } catch(...) {
                // eat up all logging failures
            }
        }

        std::shared_ptr<Logger> logger_;

        Level level_ = {};
        Tags tags_;
    };

}

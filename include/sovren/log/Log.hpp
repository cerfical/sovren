#pragma once

#include <sovren/log/ConsoleLogger.hpp>
#include <sovren/log/Level.hpp>
#include <sovren/log/Logger.hpp>
#include <sovren/log/Tags.hpp>

#include <sovren/config.hpp>

#include <functional>
#include <memory>
#include <string>
#include <type_traits>

namespace sovren::log {

    class Log {
        template <typename... Args>
        class FormatImpl;

        template <typename... Args>
        using Format = FormatImpl<std::type_identity_t<Args>...>;

        using Filter = std::function<bool(const Message&)>;

    public:
        Log() = default;

        Log(
            std::shared_ptr<Logger> logger,
            Level lvl = Config::isDebug() ? Level::Debug : Level::Info,
            Tags tags = {},
            Filter f = {}
        ) noexcept
            : logger_(std::move(logger)), filter_(std::move(f)), tags_(std::move(tags)), level_(lvl) {}

        Log(const Log&) = default;
        auto operator=(const Log&) -> Log& = default;

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
            auto l = *this;
            l.tags_ = l.tags_ + log::Tags{ std::forward<Tags>(tags)... };
            return l;
        }

        [[nodiscard]]
        auto tags() const noexcept -> const Tags& {
            return tags_;
        }

        [[nodiscard]]
        auto withLevel(Level lvl) const -> Log {
            auto l = *this;
            l.level_ = lvl;
            return l;
        }

        [[nodiscard]]
        auto level() const noexcept -> Level {
            return level_;
        }

        [[nodiscard]]
        auto withFilter(Filter f) const -> Log {
            auto l = *this;
            l.filter_ = std::move(f);
            return l;
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
        void log(Level lvl, const Format<Args...>& fmt, Args&&... args) noexcept {
            if(!logger_ || lvl > level_) {
                return;
            }

            try {
                const auto msg = fmt.makeMessage(lvl, tags_, std::forward<Args>(args)...);
                if(filter_ && !filter_(msg)) {
                    return;
                }
                logger_->write(msg);
            } catch(...) {
                // eat logging failures
            }
        }

        std::shared_ptr<Logger> logger_;

        Filter filter_;
        Tags tags_;
        Level level_ = {};
    };

    inline auto console() -> Log& {
        static auto log = Log(std::make_shared<ConsoleLogger>());
        return log;
    }

}

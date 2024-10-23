#pragma once

#include "LogKind.hpp"
#include "LogMessageFormat.hpp"
#include "LogTag.hpp"
#include "Logger.hpp"

#include <memory>
#include <type_traits>
#include <utility>

namespace sovren {

    /**
     * @brief Manages Logger instances to perform all kinds of logging activities.
     */
    class Log {
    public:

        /**
         * @brief Write a message to a log of the specified type.
         */
        template <typename... Args>
        static void log(
            LogKind kind,
            const LogTag& tag,
            const LogMessageFormat<std::type_identity_t<Args>...>& msg,
            Args&&... args
        ) noexcept {
            try {
                if(auto* const logger = get()->logger_.get()) {
                    logger->writeMessage(msg.makeMessage(tag, kind, std::forward<Args>(args)...));
                }
            } catch(...) {
                // eat all failures that can possibly occur during logging
            }
        }


        /**
         * @brief Write an untagged message to a log of the specified type.
         */
        template <typename... Args>
        static void log(LogKind kind, const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            log(kind, {}, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write a message to the debug log.
         */
        template <typename... Args>
        static void debug(const LogTag& tag, const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            log(LogKind::Debug, tag, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write an untagged message to the debug log.
         */
        template <typename... Args>
        static void debug(const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            debug({}, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write a message to the informational log.
         */
        template <typename... Args>
        static void info(const LogTag& tag, const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            log(LogKind::Info, tag, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write an untagged message to the informational log.
         */
        template <typename... Args>
        static void info(const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            info({}, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write a message to the error log.
         */
        template <typename... Args>
        static void error(const LogTag& tag, const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            log(LogKind::Error, tag, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write an untagged message to the error log.
         */
        template <typename... Args>
        static void error(const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            error({}, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write a message to the warning log.
         */
        template <typename... Args>
        static void warning(const LogTag& tag, const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            log(LogKind::Warning, tag, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write an untagged message to the warning log.
         */
        template <typename... Args>
        static void warning(const LogMessageFormat<std::type_identity_t<Args>...>& msg, Args&&... args) noexcept {
            warning({}, msg, std::forward<Args>(args)...);
        }


        Log(const Log&) = delete;
        Log& operator=(const Log&) = delete;

        Log(Log&&) = delete;
        Log& operator=(Log&&) = delete;


    private:
        static Log* get();

        Log() = default;
        ~Log() = default;

        std::unique_ptr<Logger> logger_;
    };

}

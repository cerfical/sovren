#pragma once

#include "ConsoleLogger.hpp"
#include "LogMessageFormat.hpp"
#include "Logger.hpp"

#include <type_traits>

namespace sovren {

    /**
     * @brief Responsible for all kinds of logging.
     */
    class Log {
        template <typename... Args>
        using Message = LogMessageFormat<std::type_identity_t<Args>...>;

    public:

        /**
         * @brief Write a formatted message to the debug log.
         */
        template <typename... Args>
        static void debug(const Message<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Debug, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write a formatted message to the info log.
         */
        template <typename... Args>
        static void info(const Message<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Info, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write a formatted message to the error log.
         */
        template <typename... Args>
        static void error(const Message<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Error, msg, std::forward<Args>(args)...);
        }


        /**
         * @brief Write a formatted message to the warning log.
         */
        template <typename... Args>
        static void warning(const Message<Args...>& msg, Args&&... args) noexcept {
            log(LogKind::Warning, msg, std::forward<Args>(args)...);
        }


        Log() = delete;


    private:
        template <typename... Args>
        static void log(LogKind kind, const Message<Args...>& msg, Args&&... args) noexcept {
            try {
                logger().writeMessage(msg.makeMessage(kind, std::forward<Args>(args)...));
            } catch(...) {
                // eat all failures that can possibly occur during logging
            }
        }

        static auto logger() -> Logger& {
            static ConsoleLogger logger;
            return logger;
        }
    };

}

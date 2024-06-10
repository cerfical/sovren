#pragma once

#include "LogKind.hpp"
#include "LogTag.hpp"

#include <source_location>
#include <string>
#include <utility>

namespace reni {

    /**
     * @brief Represents an instance of a single log message.
     */
    class LogMessage {
    public:

        /**
         * @brief Construct an ampty message.
         */
        LogMessage() noexcept = default;


        /**
         * @brief Construct a new message.
         */
        LogMessage(LogKind kind, LogTag tag, std::string desc, const std::source_location& loc) noexcept
            : desc_(std::move(desc)), loc_(loc), tag_(std::move(tag)), kind_(kind) {}


        /**
         * @brief Check if the message is valid.
         */
        operator bool() const noexcept {
            return !isEmpty();
        }


        /**
         * @brief Check if the message doesn't contain useful information.
         */
        bool isEmpty() const noexcept {
            return kind() == LogKind::None;
        }


        /**
         * @brief A user-friendly textual definition of the message.
         */
        const std::string& description() const noexcept {
            return desc_;
        }


        /**
         * @brief The location of the source code that generated the message.
         */
        const std::source_location& location() const noexcept {
            return loc_;
        }


        /**
         * @brief Additional information about the message to help distinguish one message from another.
         */
        const LogTag& tag() const noexcept {
            return tag_;
        }


        /**
         * @brief Type of the message.
         */
        LogKind kind() const noexcept {
            return kind_;
        }


    private:
        std::string desc_;
        std::source_location loc_;

        LogTag tag_;
        LogKind kind_ = {};
    };

}

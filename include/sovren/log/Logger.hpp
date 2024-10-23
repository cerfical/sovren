#pragma once

#include "LogMessage.hpp"

namespace sovren {

    /**
     * @brief Defines a simple interface for the actual recording of logged messages.
     */
    class Logger {
    public:

        Logger() = default;

        virtual ~Logger() = default;


        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        Logger(Logger&&) = delete;
        Logger& operator=(Logger&&) = delete;


        /**
         * @brief Make a request to log a new message.
         */
        virtual void writeMessage(const LogMessage& msg) = 0;
    };

}

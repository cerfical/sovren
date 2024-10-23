#pragma once

#include "LogMessage.hpp"

namespace sovren {

    class Logger {
    public:

        Logger() = default;

        Logger(const Logger&) = delete;
        auto operator=(const Logger&) -> Logger& = delete;

        Logger(Logger&&) = delete;
        auto operator=(Logger&&) -> Logger& = delete;

        virtual ~Logger() = default;


        virtual void writeMessage(const LogMessage& msg) = 0;
    };

}

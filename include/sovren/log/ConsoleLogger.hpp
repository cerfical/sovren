#pragma once

#include "LogMessage.hpp"
#include "Logger.hpp"

namespace sovren {

    /**
     * @brief Reports logged messages via the standard `std::clog` stream.
     */
    class ConsoleLogger : public Logger {
    public:
        void writeMessage(const LogMessage& msg) override;
    };

}

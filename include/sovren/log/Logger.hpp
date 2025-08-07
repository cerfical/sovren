#pragma once

#include "LogMessage.hpp"

namespace sovren {

    class Logger {
    public:
        virtual ~Logger() = default;

        virtual void write(const LogMessage&) = 0;
    };

}

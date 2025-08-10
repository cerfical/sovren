#pragma once

#include <sovren/log/Message.hpp>

namespace sovren::log {

    class Logger {
    public:
        virtual ~Logger() = default;

        virtual void write(const Message&) = 0;
    };

}

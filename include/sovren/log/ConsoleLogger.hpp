#pragma once

#include <sovren/log/Logger.hpp>
#include <sovren/log/Message.hpp>

#include <iostream>
#include <print>

namespace sovren::log {

    class ConsoleLogger : public Logger {
    public:
        void write(const Message& msg) override {
            std::println(std::clog, "{}", msg);
        }
    };

}

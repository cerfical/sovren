#pragma once

#include "LogMessage.hpp"
#include "Logger.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>

namespace sovren {

    class ConsoleLogger : public Logger {
    public:

        void writeMessage(const LogMessage& msg) override {
            const auto localTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());
            const auto tag = makeMessageTag(msg);

            std::clog << std::format("[{}][{}][{}]: {}\n", localTime, msg.kind, tag, msg.desc);
        }

    private:
        static auto makeMessageTag(const LogMessage& msg) -> std::string {
            // derive the name of the source file the message originated from
            const auto srcFile = std::filesystem::path(msg.loc.file_name()).filename().string();
            return std::format("{}:{}:{}", srcFile, msg.loc.column(), msg.loc.line());
        }
    };

}

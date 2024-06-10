#include "log/ConsoleLogger.hpp"

#include "log/LogKind.hpp"

#include "print.hpp"

#include <chrono>
#include <filesystem>
#include <format>
#include <iostream>
#include <sstream>
#include <string>

namespace reni {
    namespace {
        std::string stringifyKind(LogKind k) {
            return (std::ostringstream() << k).str();
        }

        std::string stringifyTag(const LogMessage& msg) {
            if(const auto& t = msg.tag()) {
                return t.value();
            }

            // derive the name of the source file the message originated from
            const auto& loc = msg.location();
            const auto srcFile = std::filesystem::path(loc.file_name()).filename().generic_string();

            return std::format("{}:{}:{}", srcFile, loc.column(), loc.line());
        }
    }

    void ConsoleLogger::writeMessage(const LogMessage& msg) {
        if(msg) {
            const auto localTime = std::chrono::current_zone()->to_local(std::chrono::system_clock::now());

            const auto kindStr = stringifyKind(msg.kind());
            const auto tagStr = stringifyTag(msg);

            std::clog << std::format("[{}][{}][{}]: {}\n", localTime, kindStr, tagStr, msg.description());
        }
    }
}

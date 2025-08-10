#pragma once

#include "LogKind.hpp"

#include <source_location>
#include <string>
#include <vector>

namespace sovren {

    struct LogMessage {
        std::string desc;
        std::source_location loc;
        std::vector<std::string> tags;
        LogKind kind = {};
    };

}

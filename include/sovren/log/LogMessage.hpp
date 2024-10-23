#pragma once

#include "LogKind.hpp"

#include <source_location>
#include <string>

namespace sovren {

    struct LogMessage {
        std::string desc;
        std::source_location loc;
        LogKind kind = {};
    };

}

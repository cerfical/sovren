#pragma once

#include <sovren/log/Level.hpp>
#include <sovren/log/Tags.hpp>

#include <source_location>
#include <string>

namespace sovren::log {

    struct Message {
        std::string desc;
        std::source_location loc;
        Tags tags;
        Level level = {};
    };

}

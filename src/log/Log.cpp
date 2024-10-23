#include "log/Log.hpp"

#include "log/ConsoleLogger.hpp"

#include "config.hpp"

namespace sovren {
    Log* Log::get() {
        static Log log;
        // for now just disable logging in debug mode
        if constexpr(Config::isDebug()) {
            if(!log.logger_) {
                log.logger_ = std::make_unique<ConsoleLogger>();
            }
        }
        return &log;
    }
}

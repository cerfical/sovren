#pragma once

#include "log/ConsoleLogger.hpp"
#include "log/Log.hpp"

#include "SceneNode.hpp"

namespace sovren {

    class LogNode : public SceneNode {
    public:
        LogNode()
            : log_(std::make_shared<ConsoleLogger>()) {}

        auto log() noexcept -> Log* override {
            return &log_;
        }

    private:
        Log log_;
    };

}

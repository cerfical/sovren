#pragma once

#include <sovren/log/ConsoleLogger.hpp>
#include <sovren/log/Log.hpp>

#include <sovren/sg/SceneNode.hpp>

#include <sovren/RenderServer.hpp>

#include <memory>
#include <optional>

namespace sovren::sg {

    class RootNode : public SceneNode {
    public:

        void init() override {
            log_ = log::Log(std::make_shared<log::ConsoleLogger>());
            log_.info("Log initialized");

            server_.emplace(log_);

            SceneNode::init();
        }

        void free() override {
            SceneNode::free();

            server_.reset();

            log_.info("Log closed");
            log_ = log::Log();
        }

        [[nodiscard]]
        auto server() noexcept -> RenderServer* override {
            return server_ ? &server_.value() : nullptr;
        }

        [[nodiscard]]
        auto log() noexcept -> log::Log* override {
            return &log_;
        }

        void exec() {
            init();

            while(server_->isActive()) {
                update();
                server_->renderFrame();
            }

            free();
        }

    private:
        log::Log log_;
        std::optional<RenderServer> server_;
    };

}

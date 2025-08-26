#pragma once

#include <sovren/log/Log.hpp>

#include <sovren/sg/SceneNode.hpp>

#include <sovren/RenderServer.hpp>

#include <optional>

namespace sovren::sg {

    class RootNode : public SceneNode {
    public:
        void init() override {
            log_ = log::console();
            log()->info("Log initialized");

            server_.emplace(*log());

            SceneNode::init();
        }

        void free() override {
            SceneNode::free();

            server_.reset();

            log()->info("Log closed");
            log_ = {};
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
                server_->prepareFrame();
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

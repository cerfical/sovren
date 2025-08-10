#pragma once

#include <sovren/log/ConsoleLogger.hpp>
#include <sovren/log/Log.hpp>

#include <sovren/pal/EventPump.hpp>

#include <sovren/rhi/Device.hpp>

#include <sovren/RenderNode.hpp>
#include <sovren/SceneNode.hpp>

#include <algorithm>
#include <memory>

namespace sovren {

    class RootNode : public RenderNode {
    public:
        void exec() {
            init();
            while(anyVisibleWindows(*this)) {
                events_->pumpEvents();
                update();
            }
            free();
        }

        void init() override {
            log_ = log::Log(std::make_shared<log::ConsoleLogger>())
                       .withTags("SG");
            log_.info("Log initialized");

            events_ = EventPump::create();
            device_ = rhi::Device::create();

            RenderNode::init();
        }

        void free() override {
            RenderNode::free();

            device_.reset();
            events_.reset();

            log_.info("Log closed");
            log_ = log::Log();
        }

        [[nodiscard]]
        auto device() noexcept -> rhi::Device* override {
            return device_.get();
        }

        [[nodiscard]]
        auto log() noexcept -> log::Log* override {
            return &log_;
        }

    private:
        [[nodiscard]]
        static auto anyVisibleWindows(const SceneNode& n) noexcept -> bool {
            return std::ranges::any_of(n.children(), [](const auto& child) {
                if(const auto n = std::dynamic_pointer_cast<RenderNode>(child)) {
                    if(const auto* const wnd = n->window()) {
                        return wnd->isVisible();
                    }
                }
                return anyVisibleWindows(*child);
            });
        }

        std::unique_ptr<EventPump> events_;
        std::unique_ptr<rhi::Device> device_;

        log::Log log_;
    };

}

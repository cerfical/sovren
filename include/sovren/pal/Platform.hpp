#pragma once

#include "../rhi/RenderBackend.hpp"

#include "EventPoller.hpp"
#include "Window.hpp"

#include <memory>

namespace sovren::pal {

    class Platform {
    public:

        static Platform* get();


        Platform(const Platform&) = delete;
        Platform& operator=(const Platform&) = delete;

        Platform(Platform&&) = delete;
        Platform& operator=(Platform&&) = delete;


        virtual std::unique_ptr<Window> createWindow() = 0;

        virtual std::unique_ptr<EventPoller> createEventPoller() = 0;

        virtual std::unique_ptr<rhi::RenderBackend> createRenderBackend() = 0;

    protected:
        Platform() = default;
        ~Platform() = default;
    };

}

#include "log/Log.hpp"

#include <sovren/pal/EventPump.hpp>
#include <sovren/pal/Window.hpp>
#include <sovren/pal/WindowHandler.hpp>

#include <sovren/pal/win32/Win32EventPump.hpp>
#include <sovren/pal/win32/Win32Window.hpp>

#include <sovren/rhi/Device.hpp>

#include <sovren/rhi/dx11/Dx11Device.hpp>

#include <memory>

namespace sovren::pal {
    auto Window::create(WindowHandler* handler, log::Log log) -> std::unique_ptr<Window> {
        return std::make_unique<Win32Window>(handler, log.withTags("Window"));
    }

    auto EventPump::create(log::Log log) -> std::unique_ptr<EventPump> {
        return std::make_unique<Win32EventPump>(log.withTags("EventPump"));
    }
}

namespace sovren::rhi {
    auto Device::create() -> std::unique_ptr<rhi::Device> {
        return std::make_unique<Dx11Device>();
    }
}
